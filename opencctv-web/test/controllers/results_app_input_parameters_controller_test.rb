require 'test_helper'

class ResultsAppInputParametersControllerTest < ActionController::TestCase
  setup do
    @results_app_input_parameter = results_app_input_parameters(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_input_parameters)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_input_parameter" do
    assert_difference('ResultsAppInputParameter.count') do
      post :create, results_app_input_parameter: { results_app_connector_parameter_id: @results_app_input_parameter.results_app_connector_parameter_id, results_app_id: @results_app_input_parameter.results_app_id, value: @results_app_input_parameter.value }
    end

    assert_redirected_to results_app_input_parameter_path(assigns(:results_app_input_parameter))
  end

  test "should show results_app_input_parameter" do
    get :show, id: @results_app_input_parameter
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_input_parameter
    assert_response :success
  end

  test "should update results_app_input_parameter" do
    patch :update, id: @results_app_input_parameter, results_app_input_parameter: { results_app_connector_parameter_id: @results_app_input_parameter.results_app_connector_parameter_id, results_app_id: @results_app_input_parameter.results_app_id, value: @results_app_input_parameter.value }
    assert_redirected_to results_app_input_parameter_path(assigns(:results_app_input_parameter))
  end

  test "should destroy results_app_input_parameter" do
    assert_difference('ResultsAppInputParameter.count', -1) do
      delete :destroy, id: @results_app_input_parameter
    end

    assert_redirected_to results_app_input_parameters_path
  end
end
