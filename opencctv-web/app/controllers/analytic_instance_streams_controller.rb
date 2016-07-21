class AnalyticInstanceStreamsController < ApplicationController
  before_action :set_analytic_instance_stream, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic_instance, only: [:index, :new, :create, :edit, :show, :update, :destroy]
  before_action :authenticate_user!
  #before_action :isOpenCCTVPageAdmin?
  respond_to :html

  # GET /analytic_instance_streams
  def index
    redirect_to analytic_instance_path(@analytic_instance)
  end

  # GET /analytic_instance_streams/1
  def show
    respond_with(@analytic_instance_stream)
  end

  # GET /analytic_instance_streams/new
  def new
    flash[:alert] = "Analytic instance may not work properly if the input streams are not properly configured"
    @analytic_instance_stream = AnalyticInstanceStream.new
    @analytic_instance_stream.analytic_instance = @analytic_instance

    #@vmses = Vms.where(user_id: current_user.id)
    @streams = Vms.where(user_id: current_user.id).joins(cameras: :streams).select("streams.id,streams.name")

    respond_with(@analytic_instance_stream)
  end

  # GET /analytic_instance_streams/1/edit
  def edit
    flash[:alert] = "Analytic instance may not work properly if the input streams are not properly configured"
  end

  # POST /analytic_instance_streams
  def create
    @analytic_instance_stream = AnalyticInstanceStream.new
    @analytic_instance_stream.analytic_instance = @analytic_instance

    if params[:analytic_instance_stream][:analytic_input_stream_id].present?
      @analytic_instance_stream.analytic_input_stream = AnalyticInputStream.find(params[:analytic_instance_stream][:analytic_input_stream_id])
    end

    if params[:analytic_instance_stream][:stream_id].present?
      @analytic_instance_stream.stream = Stream.find(params[:analytic_instance_stream][:stream_id])
    end

    @analytic_instance_stream.save

    if @analytic_instance_stream.errors.any?
      respond_with(@analytic_instance_stream)
    else
      redirect_to analytic_instance_path(@analytic_instance)
    end
  end

  # PATCH/PUT /analytic_instance_streams/1
  def update
    @analytic_instance_stream.update(analytic_input_stream_id: params[:analytic_instance_stream][:analytic_input_stream_id], stream_id: params[:analytic_instance_stream][:stream_id]   )

    if @analytic_instance_stream.errors.any?
      respond_with(@analytic_instance_stream)
    else
      redirect_to analytic_instance_path(@analytic_instance)
      #redirect_to analytic_instance_analytic_instance_stream_path(@analytic_instance, @analytic_instance_stream)
    end
  end

  # DELETE /analytic_instance_streams/1
  def destroy
    @analytic_instance_stream.destroy
    redirect_to analytic_instance_path(@analytic_instance)
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_analytic_instance_stream
      @analytic_instance_stream = AnalyticInstanceStream.find(params[:id])
    end

    def set_analytic_instance
      @analytic_instance = AnalyticInstance.find(params[:analytic_instance_id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_instance_stream_params
      params.require(:analytic_instance_stream).permit(:analytic_instance_id, :analytic_input_stream_id, :stream_id)
    end
end
