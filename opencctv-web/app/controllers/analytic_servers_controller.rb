class AnalyticServersController < ApplicationController
  before_action :set_analytic_server, only: [:show, :edit, :update, :destroy]

  respond_to :html

  def index
    @analytic_servers = AnalyticServer.all
    respond_with(@analytic_servers)
  end

  def show
    respond_with(@analytic_server)
  end

  def new
    @analytic_server = AnalyticServer.new
    respond_with(@analytic_server)
  end

  def edit
  end

  def create
    @analytic_server = AnalyticServer.new(analytic_server_params)
    @analytic_server.save
    respond_with(@analytic_server)
  end

  def update
    @analytic_server.update(analytic_server_params)
    respond_with(@analytic_server)
  end

  def destroy
    @analytic_server.destroy
    respond_with(@analytic_server)
  end

  private
    def set_analytic_server
      @analytic_server = AnalyticServer.find(params[:id])
    end

    def analytic_server_params
      params.require(:analytic_server).permit(:name, :ip, :port, :status, :pid)
    end
end
