class GoogleTokensController < ApplicationController
  protect_from_forgery with: :null_session, if: Proc.new { |c| c.request.format == 'application/json' }
  def register
    google_token = GoogleToken.new
    reg_token = google_token_params[:reg_token]
    user_email = google_token_params[:email]
    session_token = session_params[:token]
    api_session = ApiSessionToken.find_by_token(session_token)
    user = User.find_by_email(user_email)

    if api_session
      if user
        google_token.user_id = user.id
        google_token.reg_token = reg_token
        google_token.save
        render :json => {
          :response => {
            :response_code => 200,
            :response_message => "Token registered."
          }
        }
      else
        head :not_found
      end
    else
      head :unauthorized
    end
  end

  def deregister
    reg_token = google_token_params[:reg_token]
    user_email = google_token_params[:email]
    user = User.find_by_email(user_email)
    session_token = session_params[:token]
    api_session = ApiSessionToken.find_by_token(session_token)

    if api_session
      if user
        google_token = GoogleToken.find_by_reg_token(reg_token)
        if google_token
          google_token.destroy
          render :json => {
            :response => {
              :response_code => 200,
              :response_message => "Token unregistered."
            }
          }
        else
          head :unauthorized
        end
    else
      head :not_found
    end
  else
    head :unauthorized
  end

  private
    def google_token_params
      params.require(:google_token).permit(:reg_token, :email)
    end

    def session_params
      params.require(:session).permit(:token)
    end
end
