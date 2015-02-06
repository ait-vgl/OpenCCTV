class AnalyticInputStreamsController < ApplicationController
  before_filter :authenticate_user!, :except => [:index, :show]
  before_action :set_analytic_input_stream, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic, only: [:index, :new, :create, :edit, :show, :update, :destroy]
  respond_to :html

  def index
    redirect_to analytic_path(@analytic)
  end

  def show
    respond_with(@analytic_input_stream)
  end

  def new
    @analytic_input_stream = AnalyticInputStream.new
    @analytic_input_stream.analytic = @analytic
    respond_with(@analytic_input_stream)
  end

  def edit
  end

  def create
    @analytic_input_stream = @analytic.analytic_input_streams.create(name: params[:analytic_input_stream][:name], description: params[:analytic_input_stream][:description])

    if @analytic_input_stream.errors.any?
      respond_with(@analytic_input_stream)
    else
      redirect_to analytic_path(@analytic)
    end
  end

  def update
    @analytic_input_stream.update(name: params[:analytic_input_stream][:name], description: params[:analytic_input_stream][:description])

    if @analytic_input_stream.errors.any?
      respond_with(@analytic_input_stream)
    else
      redirect_to analytic_analytic_input_stream_path(@analytic, @analytic_input_stream)
    end
  end

  def destroy
    @analytic_input_stream.destroy
    redirect_to analytic_path(@analytic)
  end

  private
    def set_analytic_input_stream
      @analytic_input_stream = AnalyticInputStream.find(params[:id])
    end

  def set_analytic
    @analytic = Analytic.find(params[:analytic_id])
  end

    def analytic_input_stream_params
      params.require(:analytic_input_stream).permit(:name, :description, :analytic_id)
    end
end
