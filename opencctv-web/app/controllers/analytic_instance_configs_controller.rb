class AnalyticInstanceConfigsController < ApplicationController

  before_action :authenticate_user!

  before_action :set_analytic_instance_config, only: [:show, :edit, :update, :destroy]
  before_action :set_analytic_instance, only: [:show, :edit, :update]

  # GET /analytic_instance_configs
  # GET /analytic_instance_configs.json
  def index
    @analytic_instance_configs = AnalyticInstanceConfig.all
  end

  # GET /analytic_instance_configs/1
  # GET /analytic_instance_configs/1.json
  def show
    #@analytic_instance_config = AnalyticInstanceConfig.where(params[:id])
    #puts params[:id]
    #puts params[:analytic_instance_configs][:id]
    #puts "sssss"
    #puts params[:analytic_instance_config][:id]
  end

  # GET /analytic_instance_configs/new
  def new
    @analytic_instance_config = AnalyticInstanceConfig.new
  end

  # GET /analytic_instance_configs/1/edit
  def edit
  end

  # POST /analytic_instance_configs
  # POST /analytic_instance_configs.json
  def create
    @analytic_instance_config = AnalyticInstanceConfig.new(analytic_instance_config_params)

    respond_to do |format|
      if @analytic_instance_config.save
        format.html { redirect_to @analytic_instance_config, notice: 'Analytic instance config was successfully created.' }
        format.json { render :show, status: :created, location: @analytic_instance_config }
      else
        format.html { render :new }
        format.json { render json: @analytic_instance_config.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /analytic_instance_configs/1
  # PATCH/PUT /analytic_instance_configs/1.json
  def update
      if @analytic_instance_config.update_attributes(:data => params[:analytic_instance_config][:data])
        #format.html { redirect_to @analytic_instance_config, notice: 'Analytic instance config was successfully updated.' }
        #format.json { render :show, status: :ok, location: @analytic_instance_config }
        flash[:notice] = 'Successfully update the configuration.'
        redirect_to analytic_instance_analytic_instance_config_path(@analytic_instance,@analytic_instance_config)

      else
        flash[:error] = 'Cannot update the configuration.'
        redirect_to analytic_instance_analytic_instance_config_path(@analytic_instance,@analytic_instance_config)
        #format.html { render :edit }
        #format.json { render json: @analytic_instance_config.errors, status: :unprocessable_entity }
      end
  end

  # DELETE /analytic_instance_configs/1
  # DELETE /analytic_instance_configs/1.json
  def destroy
    @analytic_instance_config.destroy
    respond_to do |format|
      format.html { redirect_to analytic_instance_configs_url, notice: 'Analytic instance config was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_analytic_instance_config
      @analytic_instance_config = AnalyticInstanceConfig.find(params[:id])
    end

    def set_analytic_instance
      @analytic_instance = AnalyticInstance.find(params[:analytic_instance_id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def analytic_instance_config_params
      params.fetch(:analytic_instance_config, {})
    end
end
