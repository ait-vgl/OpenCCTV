require 'test_helper'

class ResultsAppConnectorFilesControllerTest < ActionController::TestCase
  setup do
    @results_app_connector_file = results_app_connector_files(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_connector_files)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_connector_file" do
    assert_difference('ResultsAppConnectorFile.count') do
      post :create, results_app_connector_file: { description: @results_app_connector_file.description, name: @results_app_connector_file.name, required: @results_app_connector_file.required, results_app_connector_id: @results_app_connector_file.results_app_connector_id }
    end

    assert_redirected_to results_app_connector_file_path(assigns(:results_app_connector_file))
  end

  test "should show results_app_connector_file" do
    get :show, id: @results_app_connector_file
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_connector_file
    assert_response :success
  end

  test "should update results_app_connector_file" do
    patch :update, id: @results_app_connector_file, results_app_connector_file: { description: @results_app_connector_file.description, name: @results_app_connector_file.name, required: @results_app_connector_file.required, results_app_connector_id: @results_app_connector_file.results_app_connector_id }
    assert_redirected_to results_app_connector_file_path(assigns(:results_app_connector_file))
  end

  test "should destroy results_app_connector_file" do
    assert_difference('ResultsAppConnectorFile.count', -1) do
      delete :destroy, id: @results_app_connector_file
    end

    assert_redirected_to results_app_connector_files_path
  end
end
