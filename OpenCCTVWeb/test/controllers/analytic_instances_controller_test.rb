require 'test_helper'

class AnalyticInstancesControllerTest < ActionController::TestCase
  setup do
    @analytic_instance = analytic_instances(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_instances)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_instance" do
    assert_difference('AnalyticInstance.count') do
      post :create, analytic_instance: { analytic_id: @analytic_instance.analytic_id, description: @analytic_instance.description, name: @analytic_instance.name }
    end

    assert_redirected_to analytic_instance_path(assigns(:analytic_instance))
  end

  test "should show analytic_instance" do
    get :show, id: @analytic_instance
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_instance
    assert_response :success
  end

  test "should update analytic_instance" do
    patch :update, id: @analytic_instance, analytic_instance: { analytic_id: @analytic_instance.analytic_id, description: @analytic_instance.description, name: @analytic_instance.name }
    assert_redirected_to analytic_instance_path(assigns(:analytic_instance))
  end

  test "should destroy analytic_instance" do
    assert_difference('AnalyticInstance.count', -1) do
      delete :destroy, id: @analytic_instance
    end

    assert_redirected_to analytic_instances_path
  end
end
