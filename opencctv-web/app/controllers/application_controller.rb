class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception

  before_action :update_sanitized_params, if: :devise_controller?

  helper_method :isOpenCCTVAdmin?, :isOpenCCTVPageAdmin?, :isOrganizationAdmin?, :isOrganizationPageAdmin?, :isOrganization?

  def update_sanitized_params
    devise_parameter_sanitizer.for(:sign_up) {|u| u.permit(:email, :password, :isAdmin, :username, :lastname)}
  end

  def isOpenCCTVAdmin?
    if current_user.isAdmin.nil?
      false
    else
        true
    end
  end


  def isOpenCCTVPageAdmin?
      if current_user.isAdmin.nil?
        flash[:error] = 'You are not allowed.'
        begin
          redirect_to :back
        rescue Exception => e
          redirect_to root_path
        end
      end
  end

  def isOrganization?
    if session[:org_id] == nil
      false
    else
      true
    end
  end


  def isOrganizationAdmin?

    user_id = current_user.id
    org_id = session[:org_id]


   if GroupUser.joins(:org_users).where(:org_id => org_id, :role_id => ApplicationController::ROLE_ORGANIZATION_ADMIN,
                                        :org_users => {:user_id => user_id}).any?
      true
    else
      false
    end
  end

  def isOrganizationPageAdmin?

    user_id = current_user.id
    org_id = session[:org_id]


    if !GroupUser.joins(:org_users).where(:org_id => org_id, :role_id => ApplicationController::ROLE_ORGANIZATION_ADMIN, :org_users => {:user_id => user_id}).any?
      flash[:error] = 'You are not allowed.'
      begin
        redirect_to :back
      rescue
        redirect_to root_path
      end
    end
  end


  def setSessionOrganizationId(org_id)
    session[:org_id] = org_id
  end

  ROLE_OPENCCTV_ADMIN = '1'
  ROLE_ORGANIZATION_ADMIN = '2'
  ROLE_RESOURCE_ADMIN = '3'

  REQUEST_STATUS_APPROVED = '1'
  REQUEST_STATUS_REJECTED = '2'
  REQUEST_STATUS_PENDING = '3'

end

