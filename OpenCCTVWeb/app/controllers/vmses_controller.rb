class VmsesController < ApplicationController
  before_action :set_vms, only: [:show, :edit, :update, :destroy]
  before_action :set_vms_connector, only: [:show, :edit, :update]
  respond_to :html

  def index
    @vmses = Vms.all
  end

  def show
  end

  def new
    @vms = Vms.new
  end

  def edit
  end

  def create
    @vms = Vms.new(vms_params)
    @vms.save
    if(!@vms.errors.any?)
      begin
        if(@vms.validate_connection)
          flash[:notice] = 'Successfully connected to the VMS.'
          @vms.add_cameras
        end
      rescue Exception => e
        flash[:error] = e.message
      end
    respond_with @vms
    end
  end

  def update
    @vms.update(vms_params)
    if !@vms.errors.any?
      begin
        if(@vms.validate_connection)
          flash[:notice] = 'Successfully connected to the VMS.'
          Camera.destroy_all(vms: @vms)
          @vms.add_cameras
        else
          @vms.set_verification(false)
        end
      rescue Exception => e
        @vms.set_verification(false)
        flash[:error] = e.message
      end
      respond_with @vms
    end
  end

  # def update
  #   @vms.update(vms_params)
  #   if !@vms.errors.any?
  #     if(@vms.vms_connector.name == 'Milestone') # for milestone vms
  #       is_vms_valid, xml_response = @vms.milestone_validate_vms
  #       if !is_vms_valid
  #         flash[:error] = "Could not connect to the Milestone VMS."
  #         redirect_to edit_vms_path(@vms)
  #       else
  #         @vms.update(:verified => true)
  #         flash[:notice] = "Successfully connected to the Milestone VMS."
  #         cameras = get_cameras(xml_response)
  #         cameras.each do |camera|
  #           existing_camera = Camera.find_by_camera_id(camera.camera_id)
  #           if(existing_camera.nil?)
  #             @vms.cameras.push(camera)
  #             if(camera.milestone_validate_camera)
  #               camera.set_verification(true)
  #             else
  #               camera.set_verification(false)
  #             end
  #           else
  #             if(existing_camera.milestone_validate_camera)
  #               existing_camera.set_verification(true)
  #             else
  #               existing_camera.set_verification(false)
  #             end
  #           end
  #         end
  #       end
  #     else # for other types of vmses
  #       # TODO
  #     end
  #   end
  #   respond_with @vms
  # end

  def destroy
    @vms.destroy
    flash[:notice] = 'VMS was successfully destroyed.'
    redirect_to vmses_url
  end

  private
  # Use callbacks to share common setup or constraints between actions.
  def set_vms
    @vms = Vms.find(params[:id])
  end

  def set_vms_connector
    @vms_connector = Vms.find(params[:id]).vms_connector
  end

  # Never trust parameters from the scary internet, only allow the white list through.
  def vms_params
    params.require(:vms).permit(:name, :description, :server_ip, :server_port, :username, :password, :vms_connector_id, :vms_type)
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