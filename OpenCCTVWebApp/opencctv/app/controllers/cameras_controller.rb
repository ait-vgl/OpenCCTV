class CamerasController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_camera, only: [:show, :edit, :update, :destroy]
  before_action :set_vms, only: [:index, :new, :create, :edit, :show, :update, :destroy]
  respond_to :html
  
  def index
    redirect_to vms_path(@vms)
  end
  
  def show
    respond_with(@camera)
  end
  
  def new
    if @vms.vms_type
      flash[:alert] = "Be careful when you add a new camera to a Milestone VMS, because when a Milestone VMS is being registered, system will automatically add all it's cameras to the system."
    end
    @camera = Camera.new
    @camera.vms = @vms
    respond_with(@camera)
  end
  
  def edit
    if @vms.vms_type
      flash[:alert] = "Be careful if you are modifying the Camera ID. If it is incorrect system will not be able to connect to the Camera through Milestone VMS."
    end
  end
  
  def create
    @camera = @vms.cameras.create(:name => params[:camera][:name], :camera_id => params[:camera][:camera_id], :description => params[:camera][:description])
    
    redirect_to vms_path(@vms)
  end
  
  def update
    @camera.update(:name => params[:camera][:name], :camera_id => params[:camera][:camera_id], :description => params[:camera][:description])
    redirect_to vms_camera_path(@vms, @camera)
  end
  
  def destroy
    @camera.destroy
    redirect_to vms_path(@vms)
  end
  
  private
  
  def set_camera
    @camera = Camera.find(params[:id])
  end
  
  def set_vms
    @vms = Vms.find(params[:vms_id])
  end
  
  def grab_frame
    
  end
end
