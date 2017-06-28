class ResultsAppConnectorsController < ApplicationController
  before_action :set_results_app_connector, only: [:show, :edit, :update, :destroy]
  before_action :authenticate_user!
  before_action :isOpenCCTVPageAdmin?

  respond_to :html

  def index
    @results_app_connectors = ResultsAppConnector.all
    respond_with(@results_app_connectors)
  end

  def show
    respond_with(@results_app_connector)
  end

  def new
    @results_app_connector = ResultsAppConnector.new
    respond_with(@results_app_connector)
  end

  def edit
  end

  def create
    @results_app_connector = ResultsAppConnector.new(results_app_connector_params)
    uploaded_io = params[:results_app_connector][:file]
    if(uploaded_io && uploaded_io.content_type.to_s == 'application/zip')
      @results_app_connector.filename = SecureRandom.uuid.to_s
      File.open(Rails.root.join('app/uploads', 'results_app_connectors', (@results_app_connector.filename  + '.zip')), 'wb') do |f|
        f.write(uploaded_io.read)
      end

      validation_result = @results_app_connector.validate_plugin_archive()
      @results_app_connector.verified = validation_result[:verified]
      if(@results_app_connector.verified?)
        flash[:notice] = 'Uploaded the new results application connector and verified.'
      else
        flash[:error] = 'Failed to verify the uploaded results application connector.'
      end

      @results_app_connector.log = validation_result[:html_content]

      @results_app_connector.save

      if (!@results_app_connector.errors.any?)
        input_files = validation_result[:input_files]
        input_files.each do |in_file|
          @results_app_connector.results_app_connector_files.push(in_file)
        end

        input_params = validation_result[:input_params]
        input_params.each do |in_param|
          @results_app_connector.results_app_connector_parameters.push(in_param)
        end
      end
    else
      flash[:error] = 'Failed to upload the new results application connector. No results application connector plugin archive (.zip) was selected or invalid file type.'
    end
    respond_with(@results_app_connector)
  end

  def update
    @results_app_connector.update(results_app_connector_params)
    respond_with(@results_app_connector)
  end

  def destroy
    path_to_file = Rails.root.join('app/uploads', 'results_app_connectors', (@results_app_connector.filename + '.zip'))
    File.delete(path_to_file) if File.exist?(path_to_file)
    @results_app_connector.destroy
    respond_with @results_app_connector
  end

  private
    def set_results_app_connector
      @results_app_connector = ResultsAppConnector.find(params[:id])
    end

    def results_app_connector_params
      #params.require(:results_app_connector).permit(:name, :description, :filename, :verified)
      params.require(:results_app_connector).permit(:name, :description)
    end
end
