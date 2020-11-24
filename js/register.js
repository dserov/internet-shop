$(document).ready(function (e) {
    $('form#form-register').submit(function (e) {
        data = $(this).serializeArray().reduce(function (obj, item) {
            obj[item.name] = item.value;
            return obj;
        }, {});
        $.post("api.php/auth/register/",
            JSON.stringify(data),
            function (otvet, textStatus, jqXHR) {
                $('#form-register').trigger("reset");
                $('#register').hide();
                $('#register-ok').show();
                show_message('Успешная регистрация!', 'true');
            });
        return false;
    })
});
