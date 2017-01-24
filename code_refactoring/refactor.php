<?php
/*
 * refactor.php
 * 
 * Copyright 2017 Santiago <santiago@santiago-XPS-L412Z>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

<?php



/*
 * Clase para manejar los mensajes
 */
class error_messages{
    public static $ERROR = 'error';
    public static $NULL_DRIVER = '1';
    public static $NULL_SERVICE = '3';
    public static $WRONG_STATUS_ID = '2';
    public static $END_POST_CONFIRM = '0';
}


/*
 * Esta clase esta para identificar a los tipos de dispositivos moviles.
 * Seria ideal tenerla por si en el futuro se quiere anadir mas tipos de dispositivos (Windows, Ubuntu)
 */
class devices_type{

    public static $IPHONE = '1';

    public static $ANDROID = '2';

    //para uso en el futuro de añadir otros sipositivos

    public static $WINDOWSPHONE = '3';

}

/*
 * Esta clase esta encargada de de enviar las notificaciones push a los usuarios.
 */
class notify_push_messages{



    public static  $SERVICIO_CONFIRMADO = 'tu servicio ha sido confirmado!';

    public static $SERVICIO_ID_NOMBRE='serviceId';


    public function notify_user($servicio){


        $push = Push::make();

        if ($servicio->user->uuid == '') {
            return Response::json(array('error' => '0'));
        }

        if ($servicio->user->type == devices_type::$IPHONE) {

            $result = $push->ios($servicio->user->uuid, notify_push_messages::$SERVICIO_CONFIRMADO, 1, 'honk.wav', 'Open', array('serviceId' => $servicio->id));

        } else {

            $result = $push->android2($servicio->user->uuid, notify_push_messages::$SERVICIO_CONFIRMADO, 1, 'default', 'Open', array(notify_push_messages::$SERVICIO_ID_NOMBRE => $servicio->id));

        }

    }

}



class actualizar_base_de_datos{

    public static $DRIVER_AVAILABLE_TRUE='1';
    public static $DRIVER_AVAILABLE_FALSE='0';


    public function update_database_post_confirn($service_id,$driver_id){

        Service::update($service_id, array(
            'driver_id' => $driver_id,
            'status_id' => '2'
        ));

        Driver::update($driver_id,array(
            'available'=>actualizar_base_de_datos::$DRIVER_AVAILABLE_FALSE
        ));

        $driverTmp = Driver::find($driver_id);


        Service::update($service_id, array(
            'car_id' => $driverTmp->car_id
        ));

    }


}


function post_confirm(){

    $id = Input:: get('service_id');
    $servicio = Service:: find($id);

    //dd($servicio);

    if ($servicio != NULL) {

        if ($servicio->status_id == '6') {

            return Response::json(array(error_messages::$ERROR => error_messages::$WRONG_STATUS_ID));

        }

        if ($servicio->driver_id == NULL && ($servicio->status_id == '1')){

            $driver_id=Input::get('driver_id');


            //Actualizar base de datos

            $updatedb=new actualizar_base_de_datos();

            $updatedb->update_database_post_confirn($id,$driver_id);




            //Notificar a usuario! !

            $servicio = Service::find($id);

            $notify_user=new notify_push_messages();

            $notify_user->notify_user($servicio);


            return Response::json(array(error_messages::$ERROR => error_messages::$END_POST_CONFIRM));

        }else {
            return Response::json(array(error_messages::$ERROR => error_messages::$NULL_DRIVER));
        }
    } else {
        return Response::json(array(error_messages::$ERROR => error_messages::$NULL_SERVICE));
    }
}


?>

