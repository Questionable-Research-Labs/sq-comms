FROM node:18 AS build

WORKDIR /app
COPY sensor-dashboard .
RUN npm install
RUN npm run build

FROM nginx:1.21.1-alpine AS deploy-static

WORKDIR /usr/share/nginx/html
RUN rm -rf ./*
COPY --from=build /app/build .

ENTRYPOINT [ "nginx", "-g", "daemon off;" ]