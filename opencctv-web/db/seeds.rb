# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)


######## Example users #################
User.create(
    username: 'admin',
    lastname: 'openCCTV',
    email: 'admin@example.com',
        password: "11111111",
        isAdmin: '1'

)

User.create(
    username: 'user',
    lastname: 'CSIM',
    email: 'user@example.com',
    password: "11111111"
)

User.create(
    username: 'user2',
    lastname: 'CSIM',
    email: 'user2@example.com',
    password: "11111111"
)

########## Requesting statuses ########

Status.create(
          status: 'Approved'
)

Status.create(
    status: 'Rejected'
)

Status.create(
    status: 'Pending'
)


############# Role ################

Role.create(
        title: 'OpenCCTVAdmin'
)

Role.create(
    title: 'OrganizationAdmin'
)


Role.create(
    title: 'ResourceAdmin'
)