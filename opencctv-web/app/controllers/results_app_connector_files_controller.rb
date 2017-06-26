class ResultsAppConnectorFilesController < ApplicationController
  before_action :authenticate_user!
  before_action :set_results_app_connector_file, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app_connector, only: [:index, :new, :create, :edit, :show, :update, :destroy]

  respond_to :html

  def index
    redirect_to results_app_connector_path(@results_app_connector)
  end

  def show
    respond_with(@results_app_connector_file)
  end

  def new
    @results_app_connector_file = ResultsAppConnectorFile.new
    @results_app_connector_file.results_app_connector = @results_app_connector
    respond_with(@results_app_connector_file)
  end

  def edit
  end

  def create
    @results_app_connector_file = @results_app_connector.results_app_connector_files.create(
        name: params[:results_app_connector_file][:name],
        required: params[:results_app_connector_file][:required],
        editable: params[:results_app_connector_file][:editable],
        description:params[:results_app_connector_file][:description])

    if @results_app_connector_file.errors.any?
      respond_with(@results_app_connector_file)
    else
      redirect_to results_app_connector_path(@results_app_connector)
    end
  end

  def update
    if(!@results_app_connector_file.results_app_input_files.empty?)
      flash[:error] = "There are results applications that are using this input file type. Therefore unable to edit details of input file #{@results_app_connector_file.name}; "
    else
      @results_app_connector_file.update(
          name: params[:results_app_connector_file][:name],
          required: params[:results_app_connector_file][:required],
          description:params[:results_app_connector_file][:description])
    end

    if @results_app_connector_file.errors.any?
        respond_with(@results_app_connector_file)
    else
      redirect_to results_app_connector_path(@results_app_connector)
    end
  end

  def destroy
    if(!@results_app_connector_file.results_app_input_files.empty?)
      flash[:error] = "There are results applications that are using this input file type. Therefore unable to delete the input file #{@results_app_connector_file.name}; "
    else
      @results_app_connector_file.destroy
    end
    redirect_to results_app_connector_path(@results_app_connector)
  end

  private
  def set_results_app_connector_file
      @results_app_connector_file = ResultsAppConnectorFile.find(params[:id])
  end

  def set_results_app_connector
    @results_app_connector = ResultsAppConnector.find(params[:results_app_connector_id])
  end

  def results_app_connector_file_params
      params.require(:results_app_connector_file).permit(:name, :required, :description, :editable, :results_app_connector_id)
  end
end
