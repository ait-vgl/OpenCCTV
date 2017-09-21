class AnalyticInstanceResultsAppsController < ApplicationController
  before_action :set_analytic_instance_results_app, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic_instance, only: [:index, :new, :create, :edit, :show, :update, :destroy]

  respond_to :html

  def index
    redirect_to analytic_instance_path(@analytic_instance)
  end

  def show
    respond_with(@analytic_instance_results_app)
  end

  def new
    @analytic_instance_results_app = AnalyticInstanceResultsApp.new
    @analytic_instance_results_app.analytic_instance = @analytic_instance
    respond_with(@analytic_instance_results_app)
  end

  def edit
    #Edit is allowed only if the analytic instance is stopped
    session[:return_to] ||= request.referer
    if @analytic_instance.status != 0
      flash[:error] = "Unable to edit results application details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to)
      else
        redirect_to @analytic_instance
      end
    end
  end

  def create
    @analytic_instance_results_app = AnalyticInstanceResultsApp.new(analytic_instance_results_app_params)
    @analytic_instance_results_app.save
    #respond_with(@analytic_instance_results_app)
    #redirect_to analytic_instance_path(@analytic_instance)

    if @analytic_instance_results_app.errors.any?
      respond_with(@analytic_instance_results_app)
    else
      redirect_to analytic_instance_path(@analytic_instance)
    end
  end

  def update
    # Update is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to edit results application details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to) and return
      else
        respond_with(@analytic_instance_results_app) and return
      end
    end

    @analytic_instance_results_app.update(analytic_instance_results_app_params)
    #respond_with(@analytic_instance_results_app)
    redirect_to analytic_instance_path(@analytic_instance)
  end

  def destroy
    # Delete is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to delete results applications registered to analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable deleting!"
      redirect_to analytic_instance_path(@analytic_instance) and return
      #respond_with(@analytic_instance) and return
    end

    @analytic_instance_results_app.destroy
    #respond_with(@analytic_instance_results_app)
    redirect_to analytic_instance_path(@analytic_instance)
  end

  private
    def set_analytic_instance_results_app
      @analytic_instance_results_app = AnalyticInstanceResultsApp.find(params[:id])
    end

    def set_analytic_instance
      @analytic_instance = AnalyticInstance.find(params[:analytic_instance_id])
    end

    def analytic_instance_results_app_params
      params.require(:analytic_instance_results_app).permit(:analytic_instance_id, :results_app_id)
    end
end
