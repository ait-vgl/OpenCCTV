class RequestsController < ApplicationController
  before_action :authenticate_user!
  before_action :isOrganizationPageAdmin?, only: [:indexAdmin, :update]

  respond_to :html


  def index
    @requests = Org.joins(requests: :status).where(:requests => {:user_id => current_user.id}).select("orgs.title, statuses.status, statuses.id as 'status_id', requests.id")
  end

  def indexAdmin
    #@requests = Request.all
    @requests_pending = Org.joins(requests: :status).where(:requests => {:org_id => session[:org_id], :status_id => ApplicationController::REQUEST_STATUS_PENDING})
                            .select("orgs.title, statuses.status, statuses.id as 'status_id', requests.id, requests.user_id")


    @requests_not_pending = Org.joins(requests: :status).where(:requests => {:org_id => session[:org_id]})
                                .where.not(:requests => {:status_id => ApplicationController::REQUEST_STATUS_PENDING})
                            .select("orgs.title, statuses.status, statuses.id as 'status_id', requests.id, requests.user_id")
  end

  def update
    @request = Request.where(:id => params[:id], :org_id => session[:org_id]).first()

    if @request.nil?
      flash[:error] = 'Status cannot be changed'
    else
      @request.update(:status_id => ApplicationController::REQUEST_STATUS_APPROVED)
      OrgUser.create(:user_id => @request.user_id, :org_id=> session[:org_id])
    end

    redirect_to org_requests_path
  end

  def show # Used for updating status to reject
    @request = Request.where(:id => params[:id], :org_id => session[:org_id]).first()

    if @request.nil?
      flash[:error] = 'Status cannot be changed'
    else
      @request.update(:status_id => ApplicationController::REQUEST_STATUS_REJECTED)

    end

    redirect_to org_requests_path
  end


  def destroy

    begin
      @request = Request.where(:id => params[:id], :user_id => current_user.id).first()
      @request.destroy


      flash[:notice] = 'Request was successfully removed.'
    rescue
      flash[:error] = 'You are not allowed to remove a request.'
    end


    redirect_to requests_path
  end

  def new ## Searching
    @orgs = Org.where("title LIKE ?", "%#{params[:text_search]}%")
                     .where.not(:id => Org.joins(:users).where(:users => {:id => current_user.id}).pluck(:id)).select("orgs.id, orgs.title, orgs.detail")

    @request = Request.new
  end

  def create

    if Request.where(:user_id => current_user.id, :org_id => params[:org_id], :status_id => ApplicationController::REQUEST_STATUS_PENDING).any?
     # redirect_to requests_path, flash: {alert: 'A request was sent already, please wait for approving.'}
      flash[:alert] = 'A request was sent already, please wait for approving.'
    else
      Request.create(:user_id => current_user.id, :org_id=> params[:org_id], :status_id => ApplicationController::REQUEST_STATUS_PENDING)



      #redirect_to requests_path,  flash: {notice: 'Request was successfully sent.'}
      flash[:notice] = 'Request was successfully sent.'
    end

    redirect_to requests_path
  end

  private

  def request_params
    params.require(:request).permit(:status_id, :org_id)
  end
end

