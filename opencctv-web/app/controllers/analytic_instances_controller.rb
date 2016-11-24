class AnalyticInstancesController < ApplicationController

  before_action :authenticate_user!

  before_action :set_analytic_instance, only: [:show, :edit, :update, :destroy, :startAnalytic, :stopAnalytic]
  before_action :setGroupList, only: [:index, :new]
  respond_to :html

  include OpenCctvServersHelper

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
  end

  # POST /analytic_instances
  def create

    if isOrganization?
      @analytic_instance = AnalyticInstance.new(:name => params[:analytic_instance][:name],
                                                :description => params[:analytic_instance][:description],
                                                :analytic_id => params[:analytic_instance][:analytic_id],
                                                :notification_id => params[:analytic_instance][:notification_id],
                                                #:user_id => current_user.id,
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
    @analytic_instance.update(analytic_instance_params)

    if isOrganization?
      redirect_to group_user_analytic_instance_path(params[:analytic_instance][:group_user_id], @analytic_instance)
    else
      respond_with(@analytic_instance)
    end

  end

  # DELETE /analytic_instances/1
  def destroy
    @analytic_instance.destroy
    respond_with(@analytic_instance)
  end


  def startAnalytic
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


  def stopAnalytic
    @openCctvServer = OpenCctvServer.first()
    @openCctvServer.port = 4445
    msg_reply = sendToServerWithData(@openCctvServer,"StopAnalytic",@analytic_instance.id, @analytic_instance.analytic_server_id)

    if(msg_reply[:type].eql? 'Error')
      flash[:error] = "#{msg_reply[:content]}"
    else
      flash[:notice] = "Stop request was sent"
      @analytic_instance.update(status: '1') # 0 = stop
    end

    redirect_to :back
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

  # Never trust parameters from the scary internet, only allow the white list through.
  def analytic_instance_params
    params.require(:analytic_instance).permit(:name, :description, :analytic_id, :notification_id, :group_user_id, :analytic_server_id)
  end
end
