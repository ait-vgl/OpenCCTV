class MobileNotificationsController < ApplicationController
  def index
    @mobile_notifications = MobileNotification.all
  end

  def new
    @mobile_notification = MobileNotification.new
    @google_projects = GoogleProject.all
    @analytics = Analytic.all
  end

  def create
    @mobile_notification = MobileNotification.new(
      :google_project_id => params[:mobile_notification][:google_project_id],
      :analytic_id => params[:mobile_notification][:analytic_id]
    )
    @mobile_notification.save
    if (!@mobile_notification.errors.any?)
      flash[:notice] = "Mobile notification is registered."
      redirect_to mobile_notifications_path
    else
      flash[:notice] = "There was some error registering mobile notification, please try again."
      flash[:color] = "invalid"
      redirect_to mobile_notifications_path
    end
  end

  def edit
  end

  def update
  end

  def destroy
    @mobile_notification = MobileNotification.find_by_id(params[:id])
    @mobile_notification.destroy
    flash[:notice] = "Mobile notification is unregistered."
    redirect_to mobile_notifications_path
  end

  def show
  end
end
