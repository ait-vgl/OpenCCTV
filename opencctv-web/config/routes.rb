Rails.application.routes.draw do

  resources :analytic_servers

  resources :analytic_instance_streams do
    resources :analytic_instance_stream_configs do
    end
  end

resources :analytic_instances do
  resources :analytic_instance_configs do
  end
end
  get 'test/createTest', to: "test#createTest"
  get 'test/clearTest'

  get 'mobile_notifications/index'

  get 'mobile_notifications/new'

  get 'mobile_notifications/create'

  get 'mobile_notifications/edit'

  get 'mobile_notifications/update'

  get 'google_projects/index'

  get 'google_projects/new'

  get 'google_projects/show'

  get 'google_projects/edit'

  get 'api_sessions/sign_in'

  get 'api_sessions/sign_out'

  get 'session/sign_out'

  get 'session/sign_in'


  get  '/org/indexUser', to: "orgs#indexUser", as: 'org_users'
  delete  '/org/removeUser/:user_id', to: "orgs#removeUser", as: 'org_remove_user'

  post '/orgs/login/:org_id', to: "orgs#loginAsOrg", as: 'login_as_org'
  delete '/orgs/login', to: "orgs#loginAsUser", as: 'login_as_user'

  post '/org_group_users/:group_user_id/:org_user_id', to: "org_group_users#create", as: 'org_group_user'
  delete '/org_group_users/:group_user_id/:org_user_id', to: "org_group_users#destroy", as: 'destroy_org_group_user'


  post '/requests/:org_id', to: "requests#create", as: 'new_org_request'
  get  '/requests/admin', to: "requests#indexAdmin", as: 'org_requests'

  resources :requests

  resources :group_users do
    resources :vmses do
    end
  end

  resources :group_users do
    resources :analytic_instances do
    end
  end

  resources :group_users do
    resources :notifications do
    end
  end

  # Overide rote of Vms
  #get '/vmses/:id/:group_user_id', to: "vmses#show", as: 'vms_org_show'

  devise_for :users

  #, controllers: {  registrations: "users/registrations", sessions: "users/sessions", }


  #devise_scope :user do
     # get '/users/sign_out', to: "users/sessions#destroy"
     # post '/users', to: "users/registrations#create"

  #end

  resources :group_users

  resources :orgs

  resources :notifications

  resources :vms_connectors



  resources :vmses do
    resources :cameras do
      resources :streams
    end
  end

  resources :analytics do
    resources :analytic_input_streams
  end


  get '/analytic_instance/:id/start', to: "analytic_instances#startAnalytic", as: 'start_analytic_instance'
  get '/analytic_instance/:id/stop', to: "analytic_instances#stopAnalytic", as: 'stop_analytic_instance'

  resources :analytic_instances do
    resources :analytic_instance_streams
  end

  resources :open_cctv_servers

  get 'about/index'

  # The priority is based upon order of creation: first created -> highest priority.
  # See how all your routes lay out with "rake routes".

  # You can have the root of your site routed with "root"
  root 'about#index'

  # Example of regular route:
  #   get 'products/:id' => 'catalog#view'

  # Example of named route that can be invoked with purchase_url(id: product.id)
  #   get 'products/:id/purchase' => 'catalog#purchase', as: :purchase

  # Example resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products


  get '/download/vms_connector/:filename' => 'download#vms_connector', as: :vms_connector_download

  get '/download/analytic/:filename' => 'download#analytic', as: :analytic_download

  get '/open_cctv_servers/:id/start' => 'open_cctv_servers#start_server', as: :open_cctv_servers_start

  get '/open_cctv_servers/:id/stop' => 'open_cctv_servers#stop_server', as: :open_cctv_servers_stop

  get '/open_cctv_servers/:id/restart' => 'open_cctv_servers#restart_server', as: :open_cctv_servers_restart

  post '/api/google_tokens/register'   => 'google_tokens#register'
  post '/api/google_tokens/deregister' => 'google_tokens#deregister'

  post '/api/users/sign_in'  => 'api_sessions#sign_in'
  post '/api/users/sign_out' => 'api_sessions#sign_out'

  resources :google_projects
  resources :mobile_notifications

  # Example resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Example resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Example resource route with more complex sub-resources:
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', on: :collection
  #     end
  #   end

  # Example resource route with concerns:
  #   concern :toggleable do
  #     post 'toggle'
  #   end
  #   resources :posts, concerns: :toggleable
  #   resources :photos, concerns: :toggleable

  # Example resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end
end
