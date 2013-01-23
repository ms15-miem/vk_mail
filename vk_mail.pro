TEMPLATE = subdirs
SUBDIRS += vk_mail
SUBDIRS += vreen

# Use .depends to specify that a project depends on another.
vk_mail.depends = vreen