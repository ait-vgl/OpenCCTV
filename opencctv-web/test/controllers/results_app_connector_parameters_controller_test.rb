require 'test_helper'

class ResultsAppConnectorParametersControllerTest < ActionController::TestCase
  setup do
    @results_app_connector_parameter = results_app_connector_parameters(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_connector_parameters)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_connector_parameter" do
    assert_difference('ResultsAppConnectorParameter.count') do
      post :create, results_app_connector_parameter: { description: @results_app_connector_parameter.description, editable: @results_app_connector_parameter.editable, name: @results_app_connector_parameter.name, required: @results_app_connector_parameter.required, results_app_connector_id: @results_app_connector_parameter.results_app_connector_id }
    end

    assert_redirected_to results_app_connector_parameter_path(assigns(:results_app_connector_parameter))
  end

  test "should show results_app_connector_parameter" do
    get :show, id: @results_app_connector_parameter
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_connector_parameter
    assert_response :success
  end

  test "should update results_app_connector_parameter" do
    patch :update, id: @results_app_connector_parameter, results_app_connector_parameter: { description: @results_app_connector_parameter.description, editable: @results_app_connector_parameter.editable, name: @results_app_connector_parameter.name, required: @results_app_connector_parameter.required, results_app_connector_id: @results_app_connector_parameter.results_app_connector_id }
    assert_redirected_to results_app_connector_parameter_path(assigns(:results_app_connector_parameter))
  end

  test "should destroy results_app_connector_parameter" do
    assert_difference('ResultsAppConnectorParameter.count', -1) do
      delete :destroy, id: @results_app_connector_parameter
    end

    assert_redirected_to results_app_connector_parameters_path
  end
end
