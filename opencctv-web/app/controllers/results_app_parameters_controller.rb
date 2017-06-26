class ResultsAppParametersController < ApplicationController
  before_action :set_results_app_parameter, only: [:show, :edit, :update, :destroy]
  before_action :set_results_app, only: [:index, :new, :create, :edit, :show, :update, :destroy]

  respond_to :html

  def index
    #@results_app_parameters = ResultsAppParameter.all
    #respond_with(@results_app_parameters)

    redirect_to results_app_path(@results_app)
  end

  def show
    respond_with(@results_app_parameter)
  end

  def new
    #@results_app_parameter = ResultsAppParameter.new
    #respond_with(@results_app_parameter)

    flash[:alert] = "Sending analytic results to the results application may not work properly if the input parameters are not properly configured"
    @results_app_parameter = ResultsAppParameter.new
    @results_app_parameter.results_app = @results_app
    respond_with(@results_app_parameter)
  end

  def edit
    flash[:alert] = "Sending analytic results to the results application may not work properly if the input parameters are not properly configured"
  end

  def create
    #@results_app_parameter = ResultsAppParameter.new(results_app_parameter_params)
    #@results_app_parameter.save
    #respond_with(@results_app_parameter)

    @results_app_parameter =  @results_app.results_app_parameters.create(
        :name => params[:results_app_parameter][:name],
        :value => params[:results_app_parameter][:value])
    redirect_to results_app_path(@results_app)
  end

  def update
=begin
    @results_app_parameter.update(results_app_parameter_params)
    respond_with(@results_app_parameter)
=end

    @results_app_parameter.update(:name => params[:results_app_parameter][:name],
                                  :value => params[:results_app_parameter][:value])
    redirect_to results_app_path(@results_app)
  end

  def destroy
    @results_app_parameter.destroy
    #respond_with(@results_app_parameter)
    # TODO : Validate if this results app is being used by an analytic instance....
    redirect_to results_app_path(@results_app)
  end

  private
    def set_results_app_parameter
      @results_app_parameter = ResultsAppParameter.find(params[:id])
    end

    def set_results_app
      @results_app = ResultsApp.find(params[:results_app_id])
    end

    def results_app_parameter_params
      params.require(:results_app_parameter).permit(:name, :value, :results_app_id)
    end
end
