class VmsesController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_vms, only: [:show, :edit, :update, :destroy]
  respond_to :html
  
  def index
    @vmses = Vms.all
    respond_with(@vmses)
  end

  def show
    respond_with(@vms)
  end

  def new
    @vms = Vms.new
    respond_with(@vms)
  end

  def edit
    if @vms.vms_type == "Milestone"
      flash[:alert] = "When you edit a Milestone VMS, system will try to reconnect to the VMS and will re-added all it's cameras to the system automatically."
    end
  end

  def create
    @vms = Vms.new(vms_params)
    @vms.save
    if !@vms.errors.any?
      if @vms.vms_type == "Milestone"
        is_vms_valid, xml_response = @vms.milestone_validate_vms
        if !is_vms_valid
          @vms.set_verification(false)
          flash[:error] = "Could not connect to the Milestone VMS."
          redirect_to edit_vms_path(@vms)
        else
          @vms.set_verification(true)
          flash[:notice] = "Successfully connected to the Milestone VMS."
          cameras = get_cameras(xml_response)
          cameras.each do |camera|
            @vms.cameras.push(camera)
            if(camera.milestone_validate_camera)
              camera.set_verification(true)
              is_default_stream_valid, default_width, default_height = camera.milestone_grab_default_stream_frame
              if(is_default_stream_valid)
                default_stream = Stream.new(:name => "Default - #{camera.name}", :width => default_width, :height => default_height, :keep_aspect_ratio => false, :allow_upsizing => false, :compression_rate => 90, :description => "Original Stream from Camera")
                default_stream.set_verification(true)
                camera.streams.push(default_stream)
              end
            else
              camera.set_verification(false)
            end
          end
          respond_with(@vms)
        end
      else
        # TODO
      end
    else
      redirect_to edit_vms_path(@vms)
    end
  end

  def update
    @vms.update(vms_params)
    if !@vms.errors.any?
      if @vms.vms_type == "Milestone"
        is_vms_valid, xml_response = @vms.milestone_validate_vms
        if !is_vms_valid
          @vms.set_verification(false)
          flash[:error] = "Could not connect to the Milestone VMS."
          redirect_to edit_vms_path(@vms)
        else
          @vms.update(:verified => true)
          flash[:notice] = "Successfully connected to the Milestone VMS."
          cameras = get_cameras(xml_response)
          cameras.each do |camera|
            existing_camera = Camera.find_by_camera_id(camera.camera_id)
            if(existing_camera.nil?)
              @vms.cameras.push(camera)
              if(camera.milestone_validate_camera)
                camera.set_verification(true)
              else
                camera.set_verification(false)
              end
            else
              if(existing_camera.milestone_validate_camera)
                existing_camera.set_verification(true)
              else
                existing_camera.set_verification(false)
              end
            end
          end
          respond_with(@vms)
        end
      else
        # TODO
      end
    else
      redirect_to edit_vms_path(@vms)
    end
  end

  def destroy
    @vms.destroy
    respond_with(@vms)
  end

  private
  def set_vms
    @vms = Vms.find(params[:id])
  end

  def vms_params
    params.require(:vms).permit(:server_name, :server_port, :vms_type, :description, :username, :password)
  end
  
  def get_cameras(xml_string)
    cameras = Array.new
    doc = Nokogiri::XML(xml_string)
    doc.xpath("//camera").each do |cam_ele|
      camera = Camera.new
      camera.name = cam_ele.attribute("cameraid").to_s
      camera.camera_id = cam_ele.xpath("guid")[0].content
      cameras.push(camera)
    end
    return cameras
  end
end
