require 'test_helper'

class ResultsAppsControllerTest < ActionController::TestCase
  setup do
    @results_app = results_apps(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_apps)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app" do
    assert_difference('ResultsApp.count') do
      post :create, results_app: { description: @results_app.description, name: @results_app.name, results_app_connector_id: @results_app.results_app_connector_id }
    end

    assert_redirected_to results_app_path(assigns(:results_app))
  end

  test "should show results_app" do
    get :show, id: @results_app
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app
    assert_response :success
  end

  test "should update results_app" do
    patch :update, id: @results_app, results_app: { description: @results_app.description, name: @results_app.name, results_app_connector_id: @results_app.results_app_connector_id }
    assert_redirected_to results_app_path(assigns(:results_app))
  end

  test "should destroy results_app" do
    assert_difference('ResultsApp.count', -1) do
      delete :destroy, id: @results_app
    end

    assert_redirected_to results_apps_path
  end
end
