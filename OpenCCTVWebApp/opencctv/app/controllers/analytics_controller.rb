class AnalyticsController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_analytic, only: [:show, :edit, :update, :destroy]
  respond_to :html

  ANALYTIC_STORE = '/usr/local/opencctv/analytics'

  def index
    @analytics = Analytic.all
    respond_with(@analytics)
  end

  def show
    respond_with(@analytic)
  end

  def new
    @analytic = Analytic.new
    respond_with(@analytic)
  end

  def edit
  end

  def create
    @analytic = Analytic.new(analytic_params)

    uploaded_io = params[:analytic][:analyticfile]
    if uploaded_io
      name = File.join(ANALYTIC_STORE, uploaded_io.original_filename)
      File.open(name, 'wb') do |file|
        file.write(uploaded_io.read)
      end
      @analytic.location = name
    end

    @analytic.save

    if !@analytic.errors.any?
      input_streams = @analytic.read_analytic_input_stream_names
      input_streams.each do |in_str|
        @analytic.analytic_input_streams.push(in_str)
      end
    end
    respond_with(@analytic)
  end

  def update
    uploaded_io = params[:analytic][:analyticfile]

    if uploaded_io
      name = File.join(ANALYTIC_STORE, uploaded_io.original_filename)
      File.open(name, 'wb') do |file|
        file.write(uploaded_io.read)
      end
      @analytic.location = name
    end

    @analytic.update(analytic_params)
    respond_with(@analytic)
  end

  def destroy
    @analytic.destroy
    respond_with(@analytic)
  end

  private
    def set_analytic
      @analytic = Analytic.find(params[:id])
    end

    def analytic_params
      params.require(:analytic).permit(:name, :description, :location)
    end
end
