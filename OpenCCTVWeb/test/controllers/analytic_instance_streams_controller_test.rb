require 'test_helper'

class AnalyticInstanceStreamsControllerTest < ActionController::TestCase
  setup do
    @analytic_instance_stream = analytic_instance_streams(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_instance_streams)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_instance_stream" do
    assert_difference('AnalyticInstanceStream.count') do
      post :create, analytic_instance_stream: { analytic_input_stream_id: @analytic_instance_stream.analytic_input_stream_id, analytic_instance_id: @analytic_instance_stream.analytic_instance_id, stream_id: @analytic_instance_stream.stream_id }
    end

    assert_redirected_to analytic_instance_stream_path(assigns(:analytic_instance_stream))
  end

  test "should show analytic_instance_stream" do
    get :show, id: @analytic_instance_stream
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_instance_stream
    assert_response :success
  end

  test "should update analytic_instance_stream" do
    patch :update, id: @analytic_instance_stream, analytic_instance_stream: { analytic_input_stream_id: @analytic_instance_stream.analytic_input_stream_id, analytic_instance_id: @analytic_instance_stream.analytic_instance_id, stream_id: @analytic_instance_stream.stream_id }
    assert_redirected_to analytic_instance_stream_path(assigns(:analytic_instance_stream))
  end

  test "should destroy analytic_instance_stream" do
    assert_difference('AnalyticInstanceStream.count', -1) do
      delete :destroy, id: @analytic_instance_stream
    end

    assert_redirected_to analytic_instance_streams_path
  end
end
