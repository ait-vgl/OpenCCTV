class AnalyticServersController < ApplicationController
  include AnalyticServersHelper

  before_action :set_analytic_server, only: [:show, :edit, :update, :destroy]

  respond_to :html

  def index
    @analytic_servers = AnalyticServer.all

    #Update the status of analytic servers
    if !@analytic_servers.nil?
      @analytic_servers.each do |analytic_server|
        reply = update_analytic_server_status(analytic_server)
      end
    end
    respond_with(@analytic_servers)
  end

  def show
    if !@analytic_server.nil?
      #Update the analytic instance status
      reply = update_analytic_inst_status(@analytic_server)

      error_msg = ""
      alert_msg = ""

      if reply[:server_status] == 'Unknown' || reply[:server_pid] == '0'
        error_msg += "Failed to retrieve status of Analytic Server #{@analytic_server.id}. "
      end

      if reply[:msg_type] == 'Error'
        error_msg += reply[:msg_content]
      elsif (reply[:msg_content] =~ /error:/i) != nil
        alert_msg += reply[:msg_content]
      end

      if !error_msg.blank?
        flash.now[:error] = error_msg
      end

      if !alert_msg.blank?
        flash.now[:alert] = alert_msg
      end
    end

    respond_with(@analytic_server)
  end

  def new
    @analytic_server = AnalyticServer.new
    respond_with(@analytic_server)
  end

  def edit
    session[:return_to] ||= request.referer
    if has_running_analytic_instances
      flash[:error] = "Unable to update detaills of analytic server #{@analytic_server.id}. There are analytic instances running on this server!"
      if session[:return_to]
        redirect_to session.delete(:return_to)
      else
        redirect_to analytic_servers_path
      end
    end
  end

  def create
    @analytic_server = AnalyticServer.new(analytic_server_params)
    @analytic_server.save

    #Update the analytic server status
    reply = update_analytic_server_status(@analytic_server)
    error_msg = ""

    if reply[:server_status] == 'Unknown' || reply[:server_pid] == '0'
      error_msg += "Failed to retrieve status of Analytic Server #{@analytic_server.id}. "
    end

    if reply[:msg_type] == 'Error'
      error_msg += reply[:msg_content]
    end

    if !error_msg.blank?
      flash.now[:error] = error_msg
    end

    respond_with(@analytic_server)
  end

  def update
    #Allow updating only if this server is currently running any analytic instances
    if has_running_analytic_instances
      flash.now[:error] = 'Unable to update the analytic server status. There are analytic instances running on this server!'
      redirect_to analytic_server_path and return
    end

    @analytic_server.update(analytic_server_params)

    #Update the analytic server status
    reply = update_analytic_server_status(@analytic_server)
    error_msg = ""

    if reply[:server_status] == 'Unknown' || reply[:server_pid] == '0'
      error_msg += "Failed to retrieve status of Analytic Server #{@analytic_server.id}. "
    end

    if reply[:msg_type] == 'Error'
      error_msg += reply[:msg_content]
    end

    if !error_msg.blank?
      flash.now[:error] = error_msg
    end

    respond_with(@analytic_server)
  end

  def destroy
    #Allow destroying only if there are no analytic instances assigned to this server
    analytic_inst = @analytic_server.analytic_instances
    if !analytic_inst.empty?
      flash.now[:error] = 'Unable to delete the analytic server. There are analytic instances assigned to this server!'
      redirect_to analytic_servers_path and return
    end

    @analytic_server.destroy
    respond_with(@analytic_server)
  end

  private
    def set_analytic_server
      @analytic_server = AnalyticServer.find(params[:id])
    end

    def has_running_analytic_instances
      analytic_inst = @analytic_server.analytic_instances.where(status: 2)
      if analytic_inst.empty?
        return false
      end
      return true
    end

    # NOT USED - DELETE LATER
    def update_status
      message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticrequest><operation>AnalyticServerStatus</operation></analyticrequest>"
      reply_msg = send_to_analytic_server(@analytic_server,message)
      @analytic_server.update(:status => reply_msg[:server_status])
      @analytic_server.update(:pid => reply_msg[:server_pid])

      if reply_msg[:server_status] == 'Unknown' || reply_msg[:server_pid] == 0
        flash.now[:error] = "Failed to retrieve status of Analytic Server #{@analytic_server.id}. "
      end
    end


    def analytic_server_params
      #params.require(:analytic_server).permit(:name, :ip, :port, :status, :pid)
      params.require(:analytic_server).permit(:name, :ip, :port)
    end
end
