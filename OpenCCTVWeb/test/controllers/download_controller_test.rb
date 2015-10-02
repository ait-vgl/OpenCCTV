require 'test_helper'

class DownloadControllerTest < ActionController::TestCase
  test "should get vms_connector" do
    get :vms_connector
    assert_response :success
  end

  test "should get analytic" do
    get :analytic
    assert_response :success
  end

end
