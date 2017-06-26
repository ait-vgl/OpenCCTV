require 'test_helper'

class ResultsAppParametersControllerTest < ActionController::TestCase
  setup do
    @results_app_parameter = results_app_parameters(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_parameters)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_parameter" do
    assert_difference('ResultsAppParameter.count') do
      post :create, results_app_parameter: { name: @results_app_parameter.name, results_app_id: @results_app_parameter.results_app_id, value: @results_app_parameter.value }
    end

    assert_redirected_to results_app_parameter_path(assigns(:results_app_parameter))
  end

  test "should show results_app_parameter" do
    get :show, id: @results_app_parameter
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_parameter
    assert_response :success
  end

  test "should update results_app_parameter" do
    patch :update, id: @results_app_parameter, results_app_parameter: { name: @results_app_parameter.name, results_app_id: @results_app_parameter.results_app_id, value: @results_app_parameter.value }
    assert_redirected_to results_app_parameter_path(assigns(:results_app_parameter))
  end

  test "should destroy results_app_parameter" do
    assert_difference('ResultsAppParameter.count', -1) do
      delete :destroy, id: @results_app_parameter
    end

    assert_redirected_to results_app_parameters_path
  end
end
