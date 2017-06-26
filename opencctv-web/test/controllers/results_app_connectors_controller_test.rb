require 'test_helper'

class ResultsAppConnectorsControllerTest < ActionController::TestCase
  setup do
    @results_app_connector = results_app_connectors(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_connectors)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_connector" do
    assert_difference('ResultsAppConnector.count') do
      post :create, results_app_connector: { description: @results_app_connector.description, filename: @results_app_connector.filename, name: @results_app_connector.name, verified: @results_app_connector.verified }
    end

    assert_redirected_to results_app_connector_path(assigns(:results_app_connector))
  end

  test "should show results_app_connector" do
    get :show, id: @results_app_connector
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_connector
    assert_response :success
  end

  test "should update results_app_connector" do
    patch :update, id: @results_app_connector, results_app_connector: { description: @results_app_connector.description, filename: @results_app_connector.filename, name: @results_app_connector.name, verified: @results_app_connector.verified }
    assert_redirected_to results_app_connector_path(assigns(:results_app_connector))
  end

  test "should destroy results_app_connector" do
    assert_difference('ResultsAppConnector.count', -1) do
      delete :destroy, id: @results_app_connector
    end

    assert_redirected_to results_app_connectors_path
  end
end
