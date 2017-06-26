class ResultsAppInputFilesController < ApplicationController
  before_action :set_results_app_input_file, only: [:show, :edit, :update, :destroy]

  respond_to :html

  def index
    @results_app_input_files = ResultsAppInputFile.all
    respond_with(@results_app_input_files)
  end

  def show
    respond_with(@results_app_input_file)
  end

  def new
    @results_app_input_file = ResultsAppInputFile.new
    respond_with(@results_app_input_file)
  end

  def edit
  end

  def create
    @results_app_input_file = ResultsAppInputFile.new(results_app_input_file_params)
    @results_app_input_file.save
    respond_with(@results_app_input_file)
  end

  def update
    @results_app_input_file.update(results_app_input_file_params)
    respond_with(@results_app_input_file)
  end

  def destroy
    @results_app_input_file.destroy
    respond_with(@results_app_input_file)
  end

  private
    def set_results_app_input_file
      @results_app_input_file = ResultsAppInputFile.find(params[:id])
    end

    def results_app_input_file_params
      params.require(:results_app_input_file).permit(:results_app_id, :results_app_connector_file_id, :filename)
    end
end
