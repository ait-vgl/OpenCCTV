ActionMailer::Base.delivery_method = :smtp
ActionMailer::Base.smtp_settings = 
{
  :address => 'smtp.gmail.com',
  :port => '587',
  :authentication => :plain,
  :user_name => 'opencctv.dev@gmail.com',
  :password => 'OpenCCTV@1',
  :domain => 'opencctv.com',
  :enable_starttls_auto => true
}
