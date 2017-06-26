require 'test_helper'

class ResultsAppInputFilesControllerTest < ActionController::TestCase
  setup do
    @results_app_input_file = results_app_input_files(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results_app_input_files)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create results_app_input_file" do
    assert_difference('ResultsAppInputFile.count') do
      post :create, results_app_input_file: { filename: @results_app_input_file.filename, results_app_connector_file_id: @results_app_input_file.results_app_connector_file_id, results_app_id: @results_app_input_file.results_app_id }
    end

    assert_redirected_to results_app_input_file_path(assigns(:results_app_input_file))
  end

  test "should show results_app_input_file" do
    get :show, id: @results_app_input_file
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @results_app_input_file
    assert_response :success
  end

  test "should update results_app_input_file" do
    patch :update, id: @results_app_input_file, results_app_input_file: { filename: @results_app_input_file.filename, results_app_connector_file_id: @results_app_input_file.results_app_connector_file_id, results_app_id: @results_app_input_file.results_app_id }
    assert_redirected_to results_app_input_file_path(assigns(:results_app_input_file))
  end

  test "should destroy results_app_input_file" do
    assert_difference('ResultsAppInputFile.count', -1) do
      delete :destroy, id: @results_app_input_file
    end

    assert_redirected_to results_app_input_files_path
  end
end
