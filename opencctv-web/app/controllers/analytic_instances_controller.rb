class AnalyticInstancesController < ApplicationController
  before_action :set_analytic_instance, only: [:show, :edit, :update, :destroy]
  respond_to :html

  # GET /analytic_instances
  def index
    @analytic_instances = AnalyticInstance.all
    respond_with(@analytic_instances)
  end

  # GET /analytic_instances/1
  def show
    respond_with(@analytic_instance)
  end

  # GET /analytic_instances/new
  def new
    @analytic_instance = AnalyticInstance.new
    respond_with(@analytic_instance)
  end

  # GET /analytic_instances/1/edit
  def edit
  end

  # POST /analytic_instances
  def create
    @analytic_instance = AnalyticInstance.new(analytic_instance_params)
    @analytic_instance.save
    respond_with(@analytic_instance)
  end

  # PATCH/PUT /analytic_instances/1
  def update
    @analytic_instance.update(analytic_instance_params)
    respond_with(@analytic_instance)
  end

  # DELETE /analytic_instances/1
  def destroy
    @analytic_instance.destroy
    respond_with(@analytic_instance)
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_analytic_instance
      @analytic_instance = AnalyticInstance.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_instance_params
      params.require(:analytic_instance).permit(:name, :description, :analytic_id)
    end
end
