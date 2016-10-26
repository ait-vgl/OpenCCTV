class NotificationsController < ApplicationController
  before_action :authenticate_user!

  before_action :set_notification, only: [:show, :edit, :update, :destroy]
  before_action :setGroupList, only: [:index]

  respond_to :html

  def index

    if isOrganization?
      @notifications = Notification.where(group_user_id: @groupList)
    else
      @notifications = Notification.where(user_id: current_user.id)
    end

    respond_with(@notifications)

  end

  def show
    respond_with(@notification)
  end

  def new
    @notification = Notification.new
    respond_with(@notification)
  end

  def edit
  end

  def create
    if isOrganization?

      @notification = Notification.new(:title => params[:notification][:title],
                                       :email => params[:notification][:email],
                                       :to_user => params[:notification][:to_user],
                                       #:user_id => current_user.id,
                                       :group_user_id => params[:notification][:group_user_id])

      @notification.save
      redirect_to group_user_notification_path(params[:notification][:group_user_id], @notification)

    else

      @notification = Notification.new(:title => params[:notification][:title],
                                       :email => params[:notification][:email],
                                       :to_user => params[:notification][:to_user],
                                       :user_id => current_user.id)

      @notification.save
      respond_with(@notification)
    end
  end

  def update
    @notification.update(notification_params)

    if isOrganization?
      redirect_to group_user_notification_path(params[:notification][:group_user_id], @notification)
    else
      respond_with(@notification)
    end

  end

  def destroy
    @notification.destroy
    respond_with(@notification)
  end

  private
  def set_notification


    if isOrganization?
      #Protecting user is not in group but try to show
      # @vms = Vms.where(:id => params[:id],
      #                 :group_user_id =>  GroupUser.joins(:org_users)
      #                                       .where(:org_id => session[:org_id],
      #                                              :org_users => {:user_id => current_user.id},
      #                                             :group_users => {:id => params[:group_user_id]}).pluck(:group_user_id)).first()


      params[:group_user_id].nil? ?
          @notification = Notification.where(:id => params[:notification][:id], :group_user_id => params[:notification][:group_user_id]).first()
      : @notification = Notification.where(:id => params[:id], :group_user_id => params[:group_user_id]).first()

    else
      @notification = Notification.where(:id => params[:id], :user_id => current_user.id).first()
    end
  end

  def setGroupList
    if isOrganization?
      @groupList = GroupUser.getGroupUserList(session[:org_id], current_user.id)
    end
  end

  def notification_params
    params.require(:notification).permit(:title, :email, :to_user, :group_user_id)
  end
end
