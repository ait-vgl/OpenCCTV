class AnalyticsController < ApplicationController
  before_action :set_analytic, only: [:show, :edit, :update, :destroy]
  before_action :authenticate_user!
  before_action :isOpenCCTVPageAdmin?
  respond_to :html

  # GET /analytics
  def index
    @analytics = Analytic.all
    respond_with @analytics
  end

  # GET /analytics/1
  def show
    respond_with @analytic
  end

  # GET /analytics/new
  def new
    @analytic = Analytic.new
    respond_with @analytic
  end

  # GET /analytics/1/edit
  def edit
  end

  # POST /analytics
  def create
    @analytic = Analytic.new(analytic_params)

    uploaded_io = params[:analytic][:file]
    if(uploaded_io && uploaded_io.content_type.to_s == 'application/zip')
      @analytic.filename = SecureRandom.uuid.to_s
      File.open(Rails.root.join('app/uploads', 'analytics', (@analytic.filename  + '.zip')), 'wb') do |f|
        f.write(uploaded_io.read)
      end

      validation_result = @analytic.validate_plugin_archive()

      @analytic.verified = validation_result[:verified]
      if(@analytic.verified?)
        flash[:notice] = 'Uploaded the new analytic and verified.'
      else
        flash[:error] = 'Failed to verify the uploaded analytic.'
      end

      @analytic.log = validation_result[:html_content]

      @analytic.save

      if (!@analytic.errors.any?)
        input_streams = validation_result[:input_streams]
        input_streams.each do |in_str|
          @analytic.analytic_input_streams.push(in_str)
        end

        analytic_configs = validation_result[:analytic_configs]
        analytic_configs.each do |in_analytic_config|
          @analytic.analytic_configs.push(in_analytic_config)
        end
      end

    else
      flash[:error] = 'Failed to upload the new analytic. No analytic plugin archive (.zip) file was selected or invalid file type.'
    end
    respond_with @analytic
  end

  # PATCH/PUT /analytics/1
  def update
    @analytic.update(analytic_params)
    respond_with @analytic
  end


  # DELETE /analytics/1
  def destroy
    path_to_file = Rails.root.join('app/uploads', 'analytics', (@analytic.filename + '.zip'))
    File.delete(path_to_file) if File.exist?(path_to_file)
    @analytic.destroy
    respond_with @analytic
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_analytic
      @analytic = Analytic.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_params
      params.require(:analytic).permit(:name, :description)
    end
end
