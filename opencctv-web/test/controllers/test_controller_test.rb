require 'test_helper'

class TestControllerTest < ActionController::TestCase
  test "should get createCameras" do
    get :createCameras
    assert_response :success
  end

end
