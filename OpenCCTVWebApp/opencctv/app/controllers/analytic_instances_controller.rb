class AnalyticInstancesController < ApplicationController
  before_action :set_analytic_instance, only: [:show, :edit, :update, :destroy]

  respond_to :html

  def index
    @analytic_instances = AnalyticInstance.all
    respond_with(@analytic_instances)
  end

  def show
    respond_with(@analytic_instance)
  end

  def new
    @analytic_instance = AnalyticInstance.new
    respond_with(@analytic_instance)
  end

  def edit
  end

  def create
    @analytic_instance = AnalyticInstance.new(analytic_instance_params)
    @analytic_instance.save
    respond_with(@analytic_instance)
  end

  def update
    @analytic_instance.update(analytic_instance_params)
    respond_with(@analytic_instance)
  end

  def destroy
    @analytic_instance.destroy
    respond_with(@analytic_instance)
  end

  private
    def set_analytic_instance
      @analytic_instance = AnalyticInstance.find(params[:id])
    end

    def analytic_instance_params
      params.require(:analytic_instance).permit(:analytic_id, :name, :description)
    end
end
