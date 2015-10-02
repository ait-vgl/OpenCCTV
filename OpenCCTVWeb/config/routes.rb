Rails.application.routes.draw do

  resources :vms_connectors

  resources :vmses do
    resources :cameras do
      resources :streams
    end
  end

  resources :analytics do
    resources :analytic_input_streams
  end

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
