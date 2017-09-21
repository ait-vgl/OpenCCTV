class AnalyticInstanceStreamsController < ApplicationController
  before_action :set_analytic_instance_stream, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic_instance, only: [:index, :new, :create, :edit, :show, :update, :destroy]
  before_action :set_stream, only: [:edit, :new, :show, :create]
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
    #TODO: add filter by group
    respond_with(@analytic_instance_stream)
  end

  # GET /analytic_instance_streams/1/edit
  def edit
    #Edit is allowed only if the analytic instance is stopped
    session[:return_to] ||= request.referer
    if @analytic_instance.status != 0
      flash[:error] = "Unable to edit video stream details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to)
      else
        redirect_to @analytic_instance
      end
    end

    flash[:alert] = "Analytic instance may not work properly if the input streams are not properly configured"
  end

  # POST /analytic_instance_streams
  def create
    @analytic_instance_stream = AnalyticInstanceStream.new(analytic_instance_stream_params)
    @analytic_instance_stream.analytic_instance = @analytic_instance
    @analytic_instance_stream.save
    if (!@analytic_instance_stream.errors.any?)
      flash[:info] = 'Analytic instance input stream added successfully'
      redirect_to analytic_instance_path(@analytic_instance)
    else
      flash[:error] = 'Failed to save analytic instance input stream details.'
      respond_with(@analytic_instance_stream)
    end
  end

  # PATCH/PUT /analytic_instance_streams/1
  def update
    # Update is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to edit video streams details of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable editing!"
      if session[:return_to]
        redirect_to session.delete(:return_to) and return
      else
        respond_with(@analytic_instance_stream) and return
      end
    end

    @analytic_instance_stream.update(analytic_input_stream_id: params[:analytic_instance_stream][:analytic_input_stream_id], stream_id: params[:analytic_instance_stream][:stream_id], config: params[:analytic_instance_stream][:config] )

    if @analytic_instance_stream.errors.any?
      respond_with(@analytic_instance_stream)
    else
      if session[:return_to]
        redirect_to session.delete(:return_to)
      else
        redirect_to analytic_instance_path(@analytic_instance)
        #redirect_to analytic_instance_analytic_instance_stream_path(@analytic_instance, @analytic_instance_stream)
      end
    end
  end

  # DELETE /analytic_instance_streams/1
  def destroy
    # Delete is allowed only if analytic has stopped
    if @analytic_instance.status != 0
      flash[:error] = "Unable to delete video streams of analytic instance #{@analytic_instance.id}. Stop the analytic instance to enable deleting!"
      redirect_to analytic_instance_path(@analytic_instance) and return
      #respond_with(@analytic_instance) and return
    end

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

    def set_stream
      if isOrganization?
        #@groupList = GroupUser.getGroupUserList(session[:org_id], current_user.id)
        @streams = Vms.where(group_user_id: session[:org_id]).joins(cameras: :streams).select("streams.id,streams.name")
      else
        @streams = Vms.where(user_id: current_user.id).joins(cameras: :streams).select("streams.id,streams.name")
      end

    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_instance_stream_params
      params.require(:analytic_instance_stream).permit(:analytic_instance_id, :analytic_input_stream_id, :stream_id, :config)
    end
end
