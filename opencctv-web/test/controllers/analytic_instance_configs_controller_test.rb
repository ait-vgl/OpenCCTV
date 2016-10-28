require 'test_helper'

class AnalyticInstanceConfigsControllerTest < ActionController::TestCase
  setup do
    @analytic_instance_config = analytic_instance_configs(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytic_instance_configs)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic_instance_config" do
    assert_difference('AnalyticInstanceConfig.count') do
      post :create, analytic_instance_config: {  }
    end

    assert_redirected_to analytic_instance_config_path(assigns(:analytic_instance_config))
  end

  test "should show analytic_instance_config" do
    get :show, id: @analytic_instance_config
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic_instance_config
    assert_response :success
  end

  test "should update analytic_instance_config" do
    patch :update, id: @analytic_instance_config, analytic_instance_config: {  }
    assert_redirected_to analytic_instance_config_path(assigns(:analytic_instance_config))
  end

  test "should destroy analytic_instance_config" do
    assert_difference('AnalyticInstanceConfig.count', -1) do
      delete :destroy, id: @analytic_instance_config
    end

    assert_redirected_to analytic_instance_configs_path
  end
end
