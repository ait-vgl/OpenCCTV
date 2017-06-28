class ResultsAppConnectorParametersController < ApplicationController
  before_action :authenticate_user!
  before_action :set_results_app_connector_parameter, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app_connector, only: [:index, :new, :create, :edit, :show, :update, :destroy]

  respond_to :html

  def index
    redirect_to results_app_connector_path(@results_app_connector)
  end

  def show
    respond_with(@results_app_connector_parameter)
  end

  def new
    @results_app_connector_parameter = ResultsAppConnectorParameter.new
    @results_app_connector_parameter.results_app_connector = @results_app_connector
    respond_with(@results_app_connector_parameter)
  end

  def edit
  end

  def create
=begin
    @results_app_connector_parameter = ResultsAppConnectorParameter.new(results_app_connector_parameter_params)
    @results_app_connector_parameter.save
    respond_with(@results_app_connector_parameter)
=end
    @results_app_connector_parameter = @results_app_connector.results_app_connector_parameters.create(
        name: params[:results_app_connector_parameter][:name],
        required: params[:results_app_connector_parameter][:required],
        editable: params[:results_app_connector_parameter][:editable],
        description:params[:results_app_connector_parameter][:description])

    if @results_app_connector_parameter.errors.any?
      respond_with(@results_app_connector_parameter)
    else
      redirect_to results_app_connector_path(@results_app_connector)
    end
  end

  def update
=begin
    @results_app_connector_parameter.update(results_app_connector_parameter_params)
    respond_with(@results_app_connector_parameter)
=end
    if(!@results_app_connector_parameter.results_app_input_parameters.empty?)
      flash[:error] = "There are results applications that are using this input parameter type. Therefore unable to edit details of input parameter #{@results_app_connector_parameter.name}; "
    else
      @results_app_connector_parameter.update(
          name: params[:results_app_connector_parameter][:name],
          required: params[:results_app_connector_parameter][:required],
          description:params[:results_app_connector_parameter][:description])
    end

    if @results_app_connector_parameter.errors.any?
      respond_with(@results_app_connector_parameter)
    else
      redirect_to results_app_connector_path(@results_app_connector)
    end
  end

  def destroy
=begin
    @results_app_connector_parameter.destroy
    respond_with(@results_app_connector_parameter)
=end
    if(!@results_app_connector_parameter.results_app_input_parameters.empty?)
      flash[:error] = "There are results applications that are using this input parameter type. Therefore unable to delete the input parameter #{@results_app_connector_parameter.name}; "
    else
      @results_app_connector_parameter.destroy
    end
    redirect_to results_app_connector_path(@results_app_connector)
  end

  private
    def set_results_app_connector_parameter
      @results_app_connector_parameter = ResultsAppConnectorParameter.find(params[:id])
    end

    def set_results_app_connector
      @results_app_connector = ResultsAppConnector.find(params[:results_app_connector_id])
    end

    def results_app_connector_parameter_params
      params.require(:results_app_connector_parameter).permit(:name, :required, :description, :editable)
    end
end
