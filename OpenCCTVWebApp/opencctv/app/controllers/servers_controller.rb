class ServersController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_server, only: [:show, :edit, :update, :destroy, :start_server, :stop_server, :restart_server]

  respond_to :html

  def index
    @server = Server.last
    if (!@server.nil?)
      request_server_status
    end
    respond_with(@server)
  end

  def show
    if (!@server.nil?)
      request_server_status
    end
    respond_with(@server)
  end

  def new
    @server = Server.new
    respond_with(@server)
  end

  def edit
  end

  def create
    @server = Server.new(server_params)
    @server.save
    respond_with(@server)
  end

  def update
    @server.status = 'Unknown'
    @server.update(server_params)
    respond_with(@server)
  end

  def destroy
    @server.destroy
    respond_with(@server)
  end

  def start_server
    exec_server_cmd('StartRequest')
  end

  def stop_server
    exec_server_cmd('StopRequest')
  end

  def restart_server
    exec_server_cmd('RestartRequest')
  end

  private
    def set_server
      @server = Server.find(params[:id])
    end

    def server_params
      params.require(:server).permit(:name, :host, :port, :status, :username, :password)
    end

    def exec_server_cmd(command)
      server_reply = @server.send_to_server(command)
      #flash[:alert] = "server_reply : #{server_reply}"

      @server.update_attribute(:status, server_reply[:server_status])
      @server.update_attribute(:pid, server_reply[:server_pid])

      if(server_reply[:type].eql? 'Error')
        flash[:alert] = "#{server_reply[:content]}"
      end
      redirect_to server_path
    end

    def request_server_status
      server_reply = @server.send_to_server('StatusRequest')
      #flash[:alert] = "server_reply : #{server_reply}"

      @server.update_attribute(:status, server_reply[:server_status])
      @server.update_attribute(:pid, server_reply[:server_pid])

      if(server_reply[:type].eql? 'Error')
        flash[:alert] = "#{server_reply[:content]}"
      end
    end

end
