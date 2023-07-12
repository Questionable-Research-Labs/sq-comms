from django.urls import path
from .views import publish_message, dashboard, api, map

urlpatterns = [
    path('publish/', publish_message, name='publish'),
    path('dashboard/', dashboard, name='Dashboard'),
    path('api/', api, name="What you doin here?"),
    path("map/", map)
]