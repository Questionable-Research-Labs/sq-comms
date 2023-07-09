from django.urls import path
from .views import publish_message, dashboard

urlpatterns = [
    path('publish/', publish_message, name='publish'),
    path('dashboard/', dashboard, name='Dashboard'),
]