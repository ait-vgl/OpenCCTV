require 'test_helper'

class AnalyticInstanceResultsAppsControllerTest < ActionController::TestCase
  setup do
    @analytic_instance_results_app = analytic_instance_results_apps(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_instance_results_apps)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_instance_results_app" do
    assert_difference('AnalyticInstanceResultsApp.count') do
      post :create, analytic_instance_results_app: { analytic_instance_id: @analytic_instance_results_app.analytic_instance_id, results_app_id: @analytic_instance_results_app.results_app_id }
    end

    assert_redirected_to analytic_instance_results_app_path(assigns(:analytic_instance_results_app))
  end

  test "should show analytic_instance_results_app" do
    get :show, id: @analytic_instance_results_app
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_instance_results_app
    assert_response :success
  end

  test "should update analytic_instance_results_app" do
    patch :update, id: @analytic_instance_results_app, analytic_instance_results_app: { analytic_instance_id: @analytic_instance_results_app.analytic_instance_id, results_app_id: @analytic_instance_results_app.results_app_id }
    assert_redirected_to analytic_instance_results_app_path(assigns(:analytic_instance_results_app))
  end

  test "should destroy analytic_instance_results_app" do
    assert_difference('AnalyticInstanceResultsApp.count', -1) do
      delete :destroy, id: @analytic_instance_results_app
    end

    assert_redirected_to analytic_instance_results_apps_path
  end
end
