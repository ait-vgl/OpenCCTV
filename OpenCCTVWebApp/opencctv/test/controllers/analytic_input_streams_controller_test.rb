require 'test_helper'

class AnalyticInputStreamsControllerTest < ActionController::TestCase
  setup do
    @analytic_input_stream = analytic_input_streams(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_input_streams)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_input_stream" do
    assert_difference('AnalyticInputStream.count') do
      post :create, analytic_input_stream: { analytic_id: @analytic_input_stream.analytic_id, description: @analytic_input_stream.description, name: @analytic_input_stream.name }
    end

    assert_redirected_to analytic_input_stream_path(assigns(:analytic_input_stream))
  end

  test "should show analytic_input_stream" do
    get :show, id: @analytic_input_stream
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_input_stream
    assert_response :success
  end

  test "should update analytic_input_stream" do
    patch :update, id: @analytic_input_stream, analytic_input_stream: { analytic_id: @analytic_input_stream.analytic_id, description: @analytic_input_stream.description, name: @analytic_input_stream.name }
    assert_redirected_to analytic_input_stream_path(assigns(:analytic_input_stream))
  end

  test "should destroy analytic_input_stream" do
    assert_difference('AnalyticInputStream.count', -1) do
      delete :destroy, id: @analytic_input_stream
    end

    assert_redirected_to analytic_input_streams_path
  end
end
