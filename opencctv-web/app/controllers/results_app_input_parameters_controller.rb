class ResultsAppInputParametersController < ApplicationController
  before_action :authenticate_user!
  before_action :set_results_app_input_parameter, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app, only: [:index, :new, :create, :edit, :show, :update, :destroy]


  respond_to :html

  def index
    redirect_to results_app_path(@results_app)
  end

  def show
    respond_with(@results_app_input_parameter)
  end

  def new
    flash[:alert] = "Results routing may not work properly if the input parameters are not properly configured !"
    @results_app_input_parameter = ResultsAppInputParameter.new
    @results_app_input_parameter.results_app = @results_app
    respond_with(@results_app_input_parameter)
  end

  def edit
    flash[:alert] = "Results routing may not work properly if the parameters files are not properly configured !"
  end

  def create
    @results_app_input_parameter = ResultsAppInputParameter.new
    @results_app_input_parameter.results_app = @results_app

    if params[:results_app_input_parameter][:results_app_connector_parameter_id].present?
      @results_app_input_parameter.results_app_connector_parameter = ResultsAppConnectorParameter.find(params[:results_app_input_parameter][:results_app_connector_parameter_id])
    end

    if params[:results_app_input_parameter][:value].present?
      @results_app_input_parameter.value = params[:results_app_input_parameter][:value]
    end

    @results_app_input_parameter.save
    @results_app.set_usable

    if @results_app_input_parameter.errors.any?
      respond_with(@results_app_input_parameter)
    else
      redirect_to results_app_path(@results_app)
    end
  end

  def update
    @results_app_input_parameter.results_app = @results_app
    @results_app_input_parameter.results_app_connector_parameter = ResultsAppConnectorParameter.find(params[:results_app_input_parameter][:results_app_connector_parameter_id])
    @results_app_input_parameter.value = params[:results_app_input_parameter][:value]

    @results_app_input_parameter.save
    @results_app.set_usable

    if @results_app_input_parameter.errors.any?
      respond_with(@results_app_input_parameter)
    else
      redirect_to results_app_path(@results_app)
    end
  end

  def destroy
    @results_app_input_parameter.destroy
    #respond_with(@results_app_input_parameter)
    @results_app.set_usable
    redirect_to results_app_path(@results_app)
  end

  private
    def set_results_app_input_parameter
      @results_app_input_parameter = ResultsAppInputParameter.find(params[:id])
    end

    def set_results_app
      @results_app = ResultsApp.find(params[:results_app_id])
    end

    def results_app_input_parameter_params
      params.require(:results_app_input_parameter).permit(:results_app_id, :results_app_connector_parameter_id, :value)
    end
end
