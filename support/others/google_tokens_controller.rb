class GoogleTokensController < ApplicationController

  def register
    email = params[:email]
    token = params[:google_token]
    user = User.find_by(email: email)
    if (user != nil)
      user_id = user.id
      google_token = GoogleToken.find_by(token: google_token)
      if (google_token == nil)
        google_token = GoogleToken.new
        google_token.token = token
        google_token.user_id = user_id
        google_token.save
      end
      head :ok
    else
      # User not found.
      head :not_found
    end
  end

  def deregister
    google_token = GoogleToken.find_by(token: params[:google_token])
    if (google_token != nil)
      google_token.destroy
      head :ok
    else
      # Google registration token not found, not the actual API.
      head :not_found
    end
  end

end
