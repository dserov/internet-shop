function show_message(msg, status) {
    var cls = "errorbox";
    if (status == 'true')
        cls = "allokbox";

    $('#message-layer-text').html(msg);
    $('#message-layer-text').attr('class', cls);
    // появление и исчезание блока
    $('#message-layer').hide()
        .clearQueue()
        .click(function () {
            $(this).hide();
            $(this).clearQueue();
        })
        .toggle(200);
    if (status == 'true')
        $('#message-layer').delay(3000).toggle(200);
}

function show_message_permanent(msg, status) {
    var cls = "errorbox";
    if (status === "true")
        cls = "allokbox";

    $('#message-layer-text').html(msg);
    $('#message-layer-text').attr('class', cls);
    // появление и исчезание блока
    $('#message-layer').hide()
        .clearQueue()
        .click(function () {
            $(this).hide();
            $(this).clearQueue();
        })
        .toggle(200);
}

$(document).ready(function (e) {
    $.ajaxSetup({
        dataType: 'json',
        beforeSend: function (jqXHR, settings) {
            $('#loading-layer').show();
        },
        complete: function (jqXHR, settings) {
            $('#loading-layer').hide();
        },
        error: function (jqXHR, textStatus, errorThrown) {
            let errorMessage = textStatus;
            if (jqXHR.responseJSON && jqXHR.responseJSON.errorMessage )
                errorMessage = jqXHR.responseJSON.errorMessage;
            if (jqXHR.responseText) {
                let data = JSON.parse(jqXHR.responseText);
                if ( typeof data !== undefined)
                    errorMessage = data.errorMessage;
            }
            show_message('Ошибка: ' + errorMessage + '<br> Код возврата: ' + jqXHR.statusText + ' (' + jqXHR.status + ')', 'false');
        }
    });
});

String.prototype.escape = function() {
    let tagsToReplace = {
        '&': '&amp;',
        '<': '&lt;',
        '>': '&gt;',
        '"': '&quot;'
    };
    return this.replace(/[&<>"]/g, function(tag) {
        return tagsToReplace[tag] || tag;
    });
};
