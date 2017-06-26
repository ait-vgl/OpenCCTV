class ResultsAppsController < ApplicationController
  before_action :authenticate_user!
  before_action :setGroupList, only: [:index]
  before_action :set_results_app, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app_connector, only: [:show, :edit, :update]

  respond_to :html

=begin
  def index
    @results_apps = ResultsApp.all
    respond_with(@results_apps)
  end
=end

  def index
    if isOrganization?
      @results_apps = ResultsApp.where(group_user_id: @groupList)
    else
      @results_apps = ResultsApp.where(user_id: current_user.id)
    end
    respond_with(@results_apps)
  end

  def show
    respond_with(@results_app)
  end

  def new
    @results_app = ResultsApp.new
    respond_with(@results_app)
  end

  def edit
  end

  def create
    @results_app = ResultsApp.new(results_app_params)
    if !(isOrganization?)
      @results_app.user_id = current_user.id
    end

    @results_app.save
    respond_with(@results_app)
  end

  def update
    @results_app.update(results_app_params)
    if !(isOrganization?)
      @results_app.user_id = current_user.id
    end
    respond_with(@results_app)
  end

  def destroy
    @results_app.destroy
    flash[:notice] = 'Results application successfully destroyed.'
    redirect_to results_apps_url
    #respond_with(@results_app)
  end

  private
    def set_results_app
      @results_app = ResultsApp.find(params[:id])
    end

    def results_app_params
      params.require(:results_app).permit(:name, :description, :results_app_connector_id, :group_user_id)
    end

    def set_results_app_connector
      @results_app_connector = ResultsApp.find(params[:id]).results_app_connector
    end

    def setGroupList
      if isOrganization?
        @groupList = GroupUser.getGroupUserList(session[:org_id], current_user.id)
      end
    end
end
