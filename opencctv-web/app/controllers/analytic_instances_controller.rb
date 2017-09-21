class AnalyticInstancesController < ApplicationController
  include AnalyticServersHelper

  before_action :authenticate_user!

  before_action :set_analytic_instance, only: [:show, :edit, :update, :destroy, :startAnalytic, :stopAnalytic]
  before_action :setGroupList, only: [:index, :new]
  respond_to :html
  #include OpenCctvServersHelper


  # GET /analytic_instances
  def index
    if isOrganization?
      @analytic_instances = AnalyticInstance.where(group_user_id: @groupList)
    else
      @analytic_instances = AnalyticInstance.where(user_id: current_user.id)
    end
    respond_with(@analytic_instances)
  end

  # GET /analytic_instances/1
  def show
    @analytic_configs = AnalyticInstanceConfig.where(analytic_instance_id: @analytic_instance.id)
    respond_with(@analytic_instance)
  end

  # GET /analytic_instances/new
  def new
   # @notifications = Notification.where(:group_user_id => @groupList)
    @analytic_instance = AnalyticInstance.new
    respond_with(@analytic_instance)
  end

  # GET /analytic_instances/1/edit
  def edit
    # Editing is allowed only if the analytic instance is stopped
    session[:return_to] ||= request.referer
    if @analytic_instance.status != 0
      flash[:error] = "Unable to update the details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to)
      else
        redirect_to analytic_instances_path
      end
    end
  end

  # POST /analytic_instances
  def create
    if isOrganization?
      @analytic_instance = AnalyticInstance.new(:name => params[:analytic_instance][:name],
                                                :description => params[:analytic_instance][:description],
                                                :analytic_id => params[:analytic_instance][:analytic_id],
                                                :notification_id => params[:analytic_instance][:notification_id],
                                                :analytic_server_id => params[:analytic_instance][:analytic_server_id],
                                                :group_user_id => params[:analytic_instance][:group_user_id])

      @analytic_instance.save

      if (!@analytic_instance.errors.any?)
        flash[:notice] = 'Successfully created analytic instance.'
        redirect_to group_user_analytic_instance_path(params[:analytic_instance][:group_user_id], @analytic_instance)
      else
        puts(@analytic_instance.errors)
        flash[:error] = 'Cannot created analytic instance.'
        redirect_to :back
      end

    else
      @analytic_instance = AnalyticInstance.new(:name => params[:analytic_instance][:name],
                                                :description => params[:analytic_instance][:description],
                                                :analytic_id => params[:analytic_instance][:analytic_id],
                                                :notification_id => params[:analytic_instance][:notification_id],
                                                :analytic_server_id => params[:analytic_instance][:analytic_server_id],
                                                :user_id => current_user.id)
      @analytic_instance.save

      respond_with(@analytic_instance)
    end

    #save  first analytic configs
    @analytic_configs = Analytic.joins(:analytic_configs).where(analytics: {id: @analytic_instance.analytic_id}).select('analytic_configs.name, analytic_configs.fileName, analytic_configs.data')
    analytic_config_array = Array.new
    @analytic_configs.each do |analytic_config|
      analytic_instance_config_db = AnalyticInstanceConfig.new
      analytic_instance_config_db.name = analytic_config.name
      analytic_instance_config_db.fileName = analytic_config.fileName
      analytic_instance_config_db.data = analytic_config.data
      @analytic_instance.analytic_instance_configs.push(analytic_instance_config_db)
    end
  end

  # PATCH/PUT /analytic_instances/1
  def update
    # Update is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to update the details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to) and return
      else
        respond_with(@analytic_instance) and return
      end
    end

    @analytic_instance.update(analytic_instance_params)
    if isOrganization?
      redirect_to group_user_analytic_instance_path(params[:analytic_instance][:group_user_id], @analytic_instance)
    else
      respond_with(@analytic_instance)
    end
  end

  # DELETE /analytic_instances/1
  def destroy
    # Delete is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to delete analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable deleting!"
      redirect_to analytic_instances_path and return
      #respond_with(@analytic_instance) and return
    end

    @analytic_instance.destroy
    respond_with(@analytic_instance)
  end

  def startAnalytic  #Sends the request directly to an anlytic server
    @analytic_server =  @analytic_instance.analytic_server
    @analytic = @analytic_instance.analytic

    if @analytic_server.nil?
      flash[:error] = "Failed to start the analytic instance #{@analytic_instance.id}, no analytic server is assigned"
    elsif @analytic.nil?
      flash[:error] = "Failed to start the analytic instance #{@analytic_instance.id}, cannot find analytic type"
    else
      message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticrequest><operation>StartAnalytic</operation>
                 <analyticinstanceid>#{@analytic_instance.id}</analyticinstanceid>
                 <analyticfilename>#{@analytic.filename}</analyticfilename>
                 </analyticrequest>"
      reply = start_analytic_inst(@analytic_server,message)

      if(reply[:msg_type].eql? 'Error')
        flash[:error] = "#{reply[:msg_content]}"
        @analytic_instance.update(status: '0') # 0 = stop, 1 = pending, 2 = started
      else
        flash[:notice] = "#{reply[:msg_content]}"
        @analytic_instance.update(status: '2')
      end
    end
    #redirect_to :back
    redirect_to analytic_instances_path
  end

  def stopAnalytic  #Sends the request directly to an anlytic server
    @analytic_server =  @analytic_instance.analytic_server

    if @analytic_server.nil?
      flash[:error] = "Failed to stop the analytic instance #{@analytic_instance.id}, no analytic server is assigned"
    else
      message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticrequest>
                 <operation>StopAnalytic</operation>
                 <analyticinstanceid>#{@analytic_instance.id}</analyticinstanceid>
                 </analyticrequest>"
      msg_reply = stop_analytic_inst(@analytic_server,message)

      if(msg_reply[:msg_type].eql? 'Error')
        flash[:error] = "#{msg_reply[:msg_content]}"
      else
        flash[:notice] = "#{msg_reply[:msg_content]}"
      end
      @analytic_instance.update(status: '0') # 0 = stop, 1 = pending, 2 = started
    end
    #redirect_to :back
    redirect_to analytic_instances_path
  end

  def startAnalytic_old
    @openCctvServer = OpenCctvServer.first()
    @openCctvServer.port = 4445
    msg_reply = sendToServerWithData(@openCctvServer,"StartAnalytic",@analytic_instance.id, @analytic_instance.analytic_server_id)

    if(msg_reply[:type].eql? 'Error')
      flash[:error] = "#{msg_reply[:content]}"
    else
      flash[:notice] = "Start request was sent"
      @analytic_instance.update(status: '1') # 0 = stop
    end
    redirect_to :back
  end

  def stopAnalytic_old
=begin
    @openCctvServer = OpenCctvServer.first()
    @openCctvServer.port = 4445
    msg_reply = sendToServerWithData(@openCctvServer,"StopAnalytic",@analytic_instance.id, @analytic_instance.analytic_server_id)

    if(msg_reply[:type].eql? 'Error')
      flash[:error] = "#{msg_reply[:content]}"
    else
      flash[:notice] = "Stop request was sent"
      #@analytic_instance.update(status: '1') # 0 = stop
      @analytic_instance.update(status: '0') # 0 = stop
    end
=end
    flash[:notice] = "Analytic instance #{@analytic_instance.id} stopped."
    @analytic_instance.update(status: '0') # 0 = stop

    #redirect_to :back
    redirect_to analytic_instances_path
  end


  private
  # Use callbacks to share common setup or constraints between actions.
  def set_analytic_instance
    if isOrganization?
      #Protecting user is not in group but try to show
      # @vms = Vms.where(:id => params[:id],
      #                 :group_user_id =>  GroupUser.joins(:org_users)
      #                                       .where(:org_id => session[:org_id],
      #                                              :org_users => {:user_id => current_user.id},
      #                                             :group_users => {:id => params[:group_user_id]}).pluck(:group_user_id)).first()

      params[:group_user_id].nil? ?
          @analytic_instance = AnalyticInstance.where(:id => params[:id], :group_user_id => params[:analytic_instance][:group_user_id]).first()
      : @analytic_instance = AnalyticInstance.where(:id => params[:id], :group_user_id => params[:group_user_id]).first()
    else
      @analytic_instance = AnalyticInstance.where(:id => params[:id], :user_id => current_user.id).first()
    end
  end

  def setGroupList # Current group per user, per org
    if isOrganization?
      @groupList = GroupUser.getGroupUserList(session[:org_id], current_user.id)
    end
  end

=begin
  def if_running_return
    session[:return_to] ||= request.referer
    if @analytic_instance.status == 1
      flash[:error] = 'Unable to update the analytic instance details. Stop the analytic instance to enable editing!'
      redirect_to session.delete(:return_to) and return
    end
  end
=end

  # Never trust parameters from the scary internet, only allow the white list through.
  def analytic_instance_params
    params.require(:analytic_instance).permit(:name, :description, :analytic_id, :notification_id, :group_user_id, :analytic_server_id)
  end
end
