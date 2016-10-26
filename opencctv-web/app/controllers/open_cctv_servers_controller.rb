class OpenCctvServersController < ApplicationController
  before_action :set_open_cctv_server, only: [:show, :edit, :update, :destroy, :start_server, :stop_server, :restart_server]
  before_action :authenticate_user!
  before_action :isOpenCCTVPageAdmin?
  respond_to :html

  # GET /open_cctv_servers
  def index
    @open_cctv_server = OpenCctvServer.last
    if (!@open_cctv_server.nil?)
      request_server_status
    end
    respond_with(@open_cctv_server)
  end

  # GET /open_cctv_servers/1
  def show
    #@open_cctv_server = OpenCctvServer.find(params[:id])
    @open_cctv_server = OpenCctvServer.last
    if (!@open_cctv_server.nil?)
      request_server_status
    end
    respond_with(@open_cctv_server)
  end

  # GET /open_cctv_servers/new
  def new
    @open_cctv_server = OpenCctvServer.new
    @open_cctv_server.name = 'OpenCCTV Server'
    @open_cctv_server.host = '127.0.0.1'
    @open_cctv_server.port = 4444
    respond_with(@open_cctv_server)
  end

  # GET /open_cctv_servers/1/edit
  def edit
  end

  # POST /open_cctv_servers
  def create
    @open_cctv_server = OpenCctvServer.new(open_cctv_server_params)
    @open_cctv_server.save
    respond_with(@open_cctv_server)
  end

  # PATCH/PUT /open_cctv_servers/1
  def update
    if(@open_cctv_server.status =='Running')
      flash[:alert] = 'It is not possible to edit the Open CCTV Server details while it is running'
    else
      @open_cctv_server.status = 'Unknown'
      @open_cctv_server.update(open_cctv_server_params)
      respond_with(@open_cctv_server)
    end
  end

  # DELETE /open_cctv_servers/1
  def destroy
    @open_cctv_server.destroy
    respond_with(@open_cctv_server)
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
  # Use callbacks to share common setup or constraints between actions.
  def set_open_cctv_server
    @open_cctv_server = OpenCctvServer.find(params[:id])

  end

  # Never trust parameters from the scary internet, only allow the white list through.
  def open_cctv_server_params
    params.require(:open_cctv_server).permit(:name, :host, :port, :status, :pid)
  end

  def exec_server_cmd(command)
    server_reply = @open_cctv_server.send_to_server(command)
    #flash[:alert] = "server_reply : #{server_reply}"

    @open_cctv_server.update_attribute(:status, server_reply[:server_status])
    @open_cctv_server.update_attribute(:pid, server_reply[:server_pid])

    if(server_reply[:type].eql? 'Error')
      flash.now[:error] = "#{server_reply[:content]}"
    end
    redirect_to open_cctv_server_path
  end

  def request_server_status
    server_reply = @open_cctv_server.send_to_server('StatusRequest')
    #flash[:alert] = "server_reply : #{server_reply}"

    @open_cctv_server.update_attribute(:status, server_reply[:server_status])
    @open_cctv_server.update_attribute(:pid, server_reply[:server_pid])

    if(server_reply[:type].eql? 'Error')
      flash.now[:error] = "#{server_reply[:content]}"
    end
  end
end