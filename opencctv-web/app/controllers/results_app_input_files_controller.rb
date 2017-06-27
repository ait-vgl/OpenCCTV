class ResultsAppInputFilesController < ApplicationController
  before_action :authenticate_user!
  before_action :set_results_app_input_file, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app, only: [:index, :new, :create, :edit, :show, :update, :destroy]

  respond_to :html

  def index
    redirect_to results_app_path(@results_app)
  end

  def show
    respond_with(@results_app_input_file)
  end

  def new
    flash[:alert] = "Results routing may not work properly if the input files are not properly configured !"
    @results_app_input_file = ResultsAppInputFile.new
    @results_app_input_file.results_app = @results_app
    respond_with(@results_app_input_file)
  end

  def edit
    flash[:alert] = "Results routing may not work properly if the input files are not properly configured !"
  end

  def create
    #Upload the file
    #If the file uploading is successful
      #set the results app
      #set the results_app_connector_file
    #render result

    @results_app_input_file = ResultsAppInputFile.new
    @results_app_input_file.results_app = @results_app

    if params[:results_app_input_file][:results_app_connector_file_id].present?
      @results_app_input_file.results_app_connector_file = ResultsAppConnectorFile.find(params[:results_app_input_file][:results_app_connector_file_id])
    end

    uploaded_io = params[:results_app_input_file][:filename]
    if(uploaded_io)
      @results_app_input_file.filename = uploaded_io.original_filename
      dir = results_app_input_file_dir + '/' + "#{@results_app.id}"
      Dir.mkdir(dir) unless Dir.exist?(dir)
      File.open(dir + '/' + uploaded_io.original_filename, 'wb') do |file|
        file.write(uploaded_io.read)
      end

      @results_app_input_file.save
      @results_app.set_usable

      if @results_app_input_file.errors.any?
        respond_with(@results_app_input_file)
      else
        redirect_to results_app_path(@results_app)
      end

    else
      flash[:error] = 'Failed to upload the input file. No file was selected'
      redirect_to results_app_path(@results_app)
    end
  end

  def update
    @results_app_input_file.results_app = @results_app

    if params[:results_app_input_file][:results_app_connector_file_id].present?
      @results_app_input_file.results_app_connector_file = ResultsAppConnectorFile.find(params[:results_app_input_file][:results_app_connector_file_id])
    end

    uploaded_io = params[:results_app_input_file][:filename]
    if(uploaded_io)
      @results_app_input_file.filename = uploaded_io.original_filename
      dir = results_app_input_file_dir + '/' + "#{@results_app.id}"
      Dir.mkdir(dir) unless Dir.exist?(dir)
      File.open(dir + '/' + uploaded_io.original_filename, 'wb') do |file|
        file.write(uploaded_io.read)
      end
    end

    @results_app_input_file.save
    @results_app.set_usable

    if @results_app_input_file.errors.any?
      respond_with(@results_app_input_file)
    else
      redirect_to results_app_path(@results_app)
    end
  end

  def destroy
    path_to_file = results_app_input_file_dir + '/' + "#{@results_app.id}" + '/' + @results_app_input_file.filename
    @results_app_input_file.destroy
    File.delete(path_to_file) if File.exist?(path_to_file)
    @results_app.set_usable
    redirect_to results_app_path(@results_app)
  end

  private
    def set_results_app_input_file
      @results_app_input_file = ResultsAppInputFile.find(params[:id])
    end

    def set_results_app
      @results_app = ResultsApp.find(params[:results_app_id])
    end

    def results_app_input_file_params
      #params.require(:results_app_input_file).permit(:results_app_id, :results_app_connector_file_id, :filename)
      params.require(:results_app_input_file).permit(:results_app_id, :results_app_connector_file_id)
    end
end
