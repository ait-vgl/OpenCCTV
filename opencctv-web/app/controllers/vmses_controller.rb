class VmsesController < ApplicationController

  before_action :authenticate_user!

  before_action :setGroupList, only: [:index]
  before_action :set_vms, only: [:show, :edit, :update, :destroy]
  before_action :set_vms_connector, only: [:show, :edit, :update]

  respond_to :html

  def index

    if isOrganization?
      @vmses = Vms.where(group_user_id: @groupList)
    else
      @vmses = Vms.where(user_id: current_user.id)
    end

    respond_with(@vmses)

  end

  def show
  end

  def new
    @vms = Vms.new
  end

  def edit
  end

  def create
    # @vms = Vms.new(vms_params, :user_id => '1')

    if isOrganization?
      @vms = Vms.new(:name => params[:vms][:name], :description => params[:vms][:description],
                     :server_ip => params[:vms][:server_ip], :server_port => params[:vms][:server_port],
                     :username => params[:vms][:username], :password => params[:vms][:password],
                     :vms_connector_id => params[:vms][:vms_connector_id], :vms_type => params[:vms][:vms_type],
                     #:user_id => current_user.id,
                     :group_user_id => params[:vms][:group_user_id])
    else
      @vms = Vms.new(:name => params[:vms][:name], :description => params[:vms][:description],
                     :server_ip => params[:vms][:server_ip], :server_port => params[:vms][:server_port],
                     :username => params[:vms][:username], :password => params[:vms][:password],
                     :vms_connector_id => params[:vms][:vms_connector_id], :vms_type => params[:vms][:vms_type],
                     :user_id => current_user.id)
    end

    @vms.save
    if (!@vms.errors.any?)
      begin
        if (@vms.validate_connection)
          flash[:notice] = 'Successfully connected to the VMS.'
          @vms.add_cameras
        end
      rescue Exception => e
        flash[:error] = e.message
      end

      if isOrganization?
        redirect_to group_user_vms_path(params[:vms][:group_user_id], @vms)
      else
        respond_with @vms
      end


    end
  end

  def update
    @vms.update(vms_params)
    if !@vms.errors.any?
      begin
        if (@vms.validate_connection)
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

      if isOrganization?
        redirect_to group_user_vms_path(params[:vms][:group_user_id], @vms)
      else
        respond_with @vms
      end

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
    if isOrganization?
      #Protecting user is not in group but try to show
      # @vms = Vms.where(:id => params[:id],
      #                 :group_user_id =>  GroupUser.joins(:org_users)
      #                                       .where(:org_id => session[:org_id],
      #                                              :org_users => {:user_id => current_user.id},
      #                                             :group_users => {:id => params[:group_user_id]}).pluck(:group_user_id)).first()


      params[:group_user_id].nil? ?
          @vms = Vms.where(:id => params[:id], :group_user_id => params[:vms][:group_user_id]).first()
      : @vms = Vms.where(:id => params[:id], :group_user_id => params[:group_user_id]).first()
    else
      @vms = Vms.where(:id => params[:id], :user_id => current_user.id).first()
    end

  end

  def set_vms_connector
    @vms_connector = Vms.find(params[:id]).vms_connector
  end


  def setGroupList
    if isOrganization?
      @groupList = GroupUser.getGroupUserList(session[:org_id], current_user.id)
    end
  end

  # Never trust parameters from the scary internet, only allow the white list through.
  def vms_params
    params.require(:vms).permit(:name, :description, :server_ip, :server_port, :username, :password, :vms_connector_id, :vms_type, :group_user_id)
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
