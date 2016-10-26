class AnalyticInputStreamsController < ApplicationController
  before_action :set_analytic_input_stream, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic, only: [:index, :new, :create, :edit, :show, :update, :destroy]
  before_action :authenticate_user!
  #before_action :isOpenCCTVPageAdmin?
  respond_to :html


  # GET /analytic_input_streams
  def index
    redirect_to analytic_path(@analytic)
  end

  # GET /analytic_input_streams/1
  def show
    respond_with(@analytic_input_stream)
  end

  # GET /analytic_input_streams/new
  def new
    @analytic_input_stream = AnalyticInputStream.new
    @analytic_input_stream.analytic = @analytic
    respond_with(@analytic_input_stream)
  end

  # GET /analytic_input_streams/1/edit
  def edit
  end

  # POST /analytic_input_streams
  def create
    @analytic_input_stream = @analytic.analytic_input_streams.create(name: params[:analytic_input_stream][:name], description: params[:analytic_input_stream][:description])

    if @analytic_input_stream.errors.any?
      respond_with(@analytic_input_stream)
    else
      redirect_to analytic_path(@analytic)
    end
  end

  # PATCH/PUT /analytic_input_streams/1
  def update
    @analytic_input_stream.update(name: params[:analytic_input_stream][:name], description: params[:analytic_input_stream][:description])

    if @analytic_input_stream.errors.any?
      respond_with(@analytic_input_stream)
    else
      #redirect_to analytic_analytic_input_stream_path(@analytic, @analytic_input_stream)
      redirect_to analytic_path(@analytic)
    end
  end

  # DELETE /analytic_input_streams/1
  def destroy
    @analytic_input_stream.destroy
    redirect_to analytic_path(@analytic)
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_analytic_input_stream
      @analytic_input_stream = AnalyticInputStream.find(params[:id])
    end

    def set_analytic
      @analytic = Analytic.find(params[:analytic_id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_input_stream_params
      params.require(:analytic_input_stream).permit(:name, :description, :analytic_id)
    end
end