!(function (e) {
    function t(t) {
        (E = -1), (k = []), (T = []);
        var s = b.limit;
        if ((t && s++, (w = e("#searchBox")[0].value), "" !== w)) {
            var i = { search: w, limit: s };
            b.pagingActive && (i.offset = A),
                e.ajax({
                    url: b.geojsonServiceAddress,
                    type: "GET",
                    data: i,
                    dataType: "json",
                    success: function (e) {
                        "Feature" === e.type ? ((D = 1), (k[0] = e), (T = e)) : ((D = e.features.length), (k = e.features), (T = s === D ? e.features.slice(0, e.features.length - 1) : e.features)), o(t), (C = t ? 1 : 0);
                    },
                    error: function () {
                        f();
                    },
                });
        }
    }
    function o(t) {
        var o = e("#searchBox").parent();
        e("#resultsDiv").remove(),
            o.append("<div id='resultsDiv' class='result'><ul id='resultList' class='list'></ul><div>"),
            (e("#resultsDiv")[0].style.position = e("#searchBox")[0].style.position),
            (e("#resultsDiv")[0].style.left = parseInt(e("#searchBox")[0].style.left) - 10 + "px"),
            (e("#resultsDiv")[0].style.bottom = e("#searchBox")[0].style.bottom),
            (e("#resultsDiv")[0].style.right = e("#searchBox")[0].style.right),
            (e("#resultsDiv")[0].style.top = parseInt(e("#searchBox")[0].style.top) + 25 + "px"),
            (e("#resultsDiv")[0].style.zIndex = e("#searchBox")[0].style.zIndex);
        var r = k.length,
            l = !1;
        t && k.length === b.limit + 1 && (r--, (l = !0), D--);
        for (var c = 0; r > c; c++) {
            var u = "<li id='listElement" + c + "' class='listResult'>";
            (u += "<span id='listElementContent" + c + "' class='content'><img src='./image/" + k[c].properties.image + "' class='iconStyle' align='middle'>"),
                (u += "<font size='2' color='#333' class='title'>" + k[c].properties.title + "</font><font size='1' color='#8c8c8c'> " + k[c].properties.description + "<font></span></li>"),
                e("#resultList").append(u),
                e("#listElement" + c).mouseenter(function () {
                    s(this);
                }),
                e("#listElement" + c).mouseleave(function () {
                    i(this);
                }),
                e("#listElement" + c).mousedown(function () {
                    n(this);
                });
        }
        if (t) {
            var p = "prev.png",
                d = "next.png",
                v = "",
                m = "";
            0 === A && ((p = "prev_dis.png"), (v = "disabled")), l || ((d = "next_dis.png"), (m = "disabled"));
            var g = "<div align='right' class='pagingDiv'>" + (A + 1) + " - " + (A + r) + " " + b.foundRecordsMessage + " ";
            (g += "<input id='pagingPrev' type='image' src='../dist/image/" + p + "' width='16' height='16' class='pagingArrow' " + v + ">"),
                (g += "<input id='pagingNext' type='image' src='../dist/image/" + d + "' width='16' height='16' class='pagingArrow' " + m + "></div>"),
                e("#resultsDiv").append(g),
                e("#pagingPrev").mousedown(function () {
                    y();
                }),
                e("#pagingNext").mousedown(function () {
                    h();
                }),
                a();
        }
    }
    function s(t) {
        var o = parseInt(t.id.substr(11));
        o !== E && e("#listElement" + o).toggleClass("mouseover");
    }
    function i(t) {
        var o = parseInt(t.id.substr(11));
        o !== E && e("#listElement" + o).removeClass("mouseover");
    }
    function n(t) {
        var o = parseInt(t.id.substr(11));
        o !== E && (-1 !== E && e("#listElement" + E).removeClass("active"), e("#listElement" + o).removeClass("mouseover"), e("#listElement" + o).addClass("active"), (E = o), p(), 0 === C ? c(E) : r(E));
    }
    function a() {
        void 0 !== B && (map.removeLayer(B), (B = void 0)),
            (B = L.geoJson(T, {
                style: function (e) {
                    return { color: "#D0473B" };
                },
                pointToLayer: function (e, t) {
                    if (e.properties.tag === "hut") {
                        return new L.Marker(t, {icon: HutIcon});
                    }
                    else {
                        return new L.Marker(t, {icon: CampIcon});
                    }
                },
                onEachFeature: function (e, t) {
                    t.bindPopup(e.properties.popupContent, {maxWidth: 500});
                },
            })),
            map.addLayer(B),
            map.fitBounds(B.getBounds());
    }
    function r(e) {
        "Point" === k[e].geometry.type && -1 !== b.pointGeometryZoomLevel ? map.setView([k[e].geometry.coordinates[1], k[e].geometry.coordinates[0]], b.pointGeometryZoomLevel) : map.fitBounds(l(k[e].geometry)), u(e);
    }
    function l(e) {
        var t = L.geoJson(e, { onEachFeature: function (e, t) {} });
        return t.getBounds();
    }
    function c(e) {
        if ((void 0 !== B && (map.removeLayer(B), (B = void 0)), -1 !== e)) {
            var t = { color: b.drawColor, weight: 5, opacity: 0.65, fill: !1 };
            (B = L.geoJson(k[e].geometry, {
                style: t,
                onEachFeature: function (t, o) {
                    o.bindPopup(k[e].properties.popupContent, {maxWidth: 500});
                },
                pointToLayer: function(geoJsonPoint, latlng) {
        if (geoJsonPoint.properties.tag === "hut") {
            return L.marker(latlng, {keyboard: false, icon: HutIcon});
        }
        else {
            return L.marker(latlng, {keyboard: false, icon: CampIcon})
        }
}
            })),
                map.addLayer(B),
                "Point" === k[e].geometry.type && -1 !== b.pointGeometryZoomLevel ? map.setView([k[e].geometry.coordinates[1], k[e].geometry.coordinates[0]], b.pointGeometryZoomLevel) : map.fitBounds(B.getBounds());
        }
    }
    function u(e) {
        if ((void 0 !== x && (map.removeLayer(x), (x = void 0)), -1 !== e)) {
            var t = { color: b.color, weight: 5, opacity: 0.65, fill: !1 };
            (x = L.geoJson(k[e].geometry, {
                style: t,
                onEachFeature: function (t, o) {
                    o.bindPopup(k[e].properties.popupContent, {maxWidth: 500});
                },
                pointToLayer: function(geoJsonPoint, latlng) {
        if (geoJsonPoint.properties.tag === "hut") {
            return L.marker(latlng, {keyboard: false, icon: HutIcon});
        }
        else {
            return L.marker(latlng, {keyboard: false, icon: CampIcon})
        }
}
            })),
                map.addLayer(x);
        }
    }
    function p() {
        -1 === E ? (e("#searchBox")[0].value = w) : (e("#searchBox")[0].value = k[E].properties.title);
    }
    function d() {
        D > 0 && (-1 !== E && e("#listElement" + E).toggleClass("active"), D - 1 > E ? (e("#listElement" + (E + 1)).toggleClass("active"), E++) : (E = -1), p(), -1 !== E && (0 === C ? c(E) : r(E)));
    }
    function v() {
        D > 0 &&
            (-1 !== E && e("#listElement" + E).toggleClass("active"),
            -1 === E ? (e("#listElement" + (D - 1)).toggleClass("active"), (E = D - 1)) : 0 === E ? E-- : (e("#listElement" + (E - 1)).toggleClass("active"), E--),
            p(),
            -1 !== E && (0 === C ? c(E) : r(E)));
    }
    function m() {
        (e("#searchBox")[0].value = ""), (w = ""), (D = 0), (k = []), (E = -1), e("#resultsDiv").remove(), void 0 !== B && (map.removeLayer(B), (B = void 0)), void 0 !== x && (map.removeLayer(x), (x = void 0));
    }
    function g() {
        t(b.pagingActive);
    }
    function f() {
        (D = 0), (k = []), (E = -1), e("#resultsDiv").remove(), void 0 !== B && (map.removeLayer(B), (B = void 0));
        var t = e("#searchBox").parent();
        e("#resultsDiv").remove(), t.append("<div id='resultsDiv' class='result'><i>" + w + " " + b.notFoundMessage + " <p><small>" + b.notFoundHint + "</small></i><div>");
    }
    function y() {
        (e("#searchBox")[0].value = w), (A -= b.limit), t(!0), (F = !1), (E = -1);
    }
    function h() {
        (e("#searchBox")[0].value = w), (A += b.limit), t(!0), (F = !1), (E = -1);
    }
    var B,
        x,
        C,
        b = {
            geojsonServiceAddress: "http://yourGeoJsonSearchAddress",
            placeholderMessage: "Search...",
            searchButtonTitle: "Search",
            clearButtonTitle: "Clear",
            foundRecordsMessage: "showing results.",
            limit: 15,
            notFoundMessage: "not found.",
            notFoundHint: "Make sure your search criteria is correct and try again.",
            drawColor: "orange",
            pointGeometryZoomLevel: -1,
            pagingActive: !0,
        },
        E = -1,
        D = 0,
        w = "",
        k = [],
        T = [],
        A = 0,
        F = !0;
    (e.fn.GeoJsonAutocomplete = function (o) {
        for (var s = Object.keys(o), i = 0; i < s.length; i++) b[s[i]] = o[s[i]];
        e(this).each(function () {
            var o = e(this);
            o.addClass("searchContainer"),
                o.append('<input id="searchBox" class="searchBox" placeholder="' + b.placeholderMessage + '"/>'),
                o.append('<input id="searchButton" class="searchButton" type="submit" value="" title="' + b.searchButtonTitle + '"/>'),
                o.append('<span class="divider"></span>'),
                o.append('<input id="clearButton" class="clearButton" type="submit"  value="" title="' + b.clearButtonTitle + '">'),
                (e("#searchBox")[0].value = ""),
                e("#searchBox").delayKeyup(function (o) {
                    switch (o.keyCode) {
                        case 13:
                            g();
                            break;
                        case 38:
                            v();
                            break;
                        case 40:
                            d();
                            break;
                        case 37:
                        case 39:
                            break;
                        default:
                            e("#searchBox")[0].value.length > 0 ? ((A = 0), t(!1)) : m();
                    }
                }, 300),
                e("#searchBox").focus(function () {
                    void 0 !== e("#resultsDiv")[0] && (e("#resultsDiv")[0].style.visibility = "visible");
                }),
                e("#searchBox").blur(function () {
                    void 0 !== e("#resultsDiv")[0] &&
                        (F
                            ? (e("#resultsDiv")[0].style.visibility = "collapse")
                            : ((F = !0),
                              window.setTimeout(function () {
                                  e("#searchBox").focus();
                              }, 0)));
                }),
                e("#searchButton").click(function () {
                    g();
                }),
                e("#clearButton").click(function () {
                    m();
                });
        });
    }),
        (e.fn.delayKeyup = function (t, o) {
            var s = 0;
            return (
                e(this).keyup(function (e) {
                    13 !== e.keyCode && 38 !== e.keyCode && 40 !== e.keyCode
                        ? (clearTimeout(s),
                          (s = setTimeout(function () {
                              t(e);
                          }, o)))
                        : t(e);
                }),
                e(this)
            );
        });
})(jQuery);
