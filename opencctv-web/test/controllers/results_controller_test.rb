require 'test_helper'

class ResultsControllerTest < ActionController::TestCase
  setup do
    @result = results(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:results)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create result" do
    assert_difference('Result.count') do
      post :create, result: { analytic_id: @result.analytic_id, analytic_instance_id: @result.analytic_instance_id, camera_id: @result.camera_id, location: @result.location, result_text: @result.result_text, timestamp: @result.timestamp, vms_id: @result.vms_id }
    end

    assert_redirected_to result_path(assigns(:result))
  end

  test "should show result" do
    get :show, id: @result
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @result
    assert_response :success
  end

  test "should update result" do
    patch :update, id: @result, result: { analytic_id: @result.analytic_id, analytic_instance_id: @result.analytic_instance_id, camera_id: @result.camera_id, location: @result.location, result_text: @result.result_text, timestamp: @result.timestamp, vms_id: @result.vms_id }
    assert_redirected_to result_path(assigns(:result))
  end

  test "should destroy result" do
    assert_difference('Result.count', -1) do
      delete :destroy, id: @result
    end

    assert_redirected_to results_path
  end
end
