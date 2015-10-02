require 'test_helper'

class AnalyticsControllerTest < ActionController::TestCase
  setup do
    @analytic = analytics(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:analytics)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create analytic" do
    assert_difference('Analytic.count') do
      post :create, analytic: { description: @analytic.description, filename: @analytic.filename, name: @analytic.name, verified: @analytic.verified }
    end

    assert_redirected_to analytic_path(assigns(:analytic))
  end

  test "should show analytic" do
    get :show, id: @analytic
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @analytic
    assert_response :success
  end

  test "should update analytic" do
    patch :update, id: @analytic, analytic: { description: @analytic.description, filename: @analytic.filename, name: @analytic.name, verified: @analytic.verified }
    assert_redirected_to analytic_path(assigns(:analytic))
  end

  test "should destroy analytic" do
    assert_difference('Analytic.count', -1) do
      delete :destroy, id: @analytic
    end

    assert_redirected_to analytics_path
  end
end
