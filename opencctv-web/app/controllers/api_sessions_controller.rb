class ApiSessionsController < ApplicationController
  protect_from_forgery with: :null_session, if: Proc.new { |c| c.request.format == 'application/json' }

  def sign_in
    @user = User.find_by_email(params[:user][:email])
    password_valid = @user.valid_password?(params[:user][:password])
    if password_valid
      logger.info("  [INFO] Sign in request from authorized user.")
      random_token = ''
      while true
        random_token = SecureRandom.urlsafe_base64(nil, false)
        token_existed = ApiSessionToken.exists?(token: random_token)
        if !token_existed
          break
        end
      end
      logger.info("  [INFO] Session Token: " + random_token)
      session_token = ApiSessionToken.new
      session_token.token = random_token
      session_token.user_id = @user.id
      session_token.save
      render :json => {:session => {:token => random_token}}
    else
      logger.info("  [INFO] Sign in request from unauthorized user.")
      head :unauthorized
    end
  end

  def sign_out
    session_token = ApiSessionToken.find_by_token(params[:session][:token])
    if session_token != nil
      logger.info("  [INFO] Intended to log out")
      session_token.destroy
      head :ok
    else
      head :unauthorized
    end
  end
end
